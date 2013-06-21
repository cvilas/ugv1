//==============================================================================
// Project  : UGV1
// Module   : ControllerAgent
// File     : ControllerAgent.h
// Brief    : Robot controller
//==============================================================================

#ifndef UGV1_CONTROLLERAGENT_H
#define	UGV1_CONTROLLERAGENT_H

#ifdef UGV1CONTROLLERAGENT_DLL
#	ifdef UGV1CONTROLLERAGENT_DLL_EXPORT
#		define UGV1CONTROLLERAGENT_DLL_API __declspec(dllexport)
#	else
#		define UGV1CONTROLLERAGENT_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1CONTROLLERAGENT_DLL_API
#endif


#include "AgentThread.h"
#include "RobotModel.h"
#include "SerialPort.h"
#include "AgentMessenger.h"
#include "Ugv1Messages/JoyMessage.hpp"
#include "Ugv1Messages/OdometryMessage.hpp"
#include "Ugv1Messages/CommandMessage.hpp"
#include "Ugv1Messages/HealthMessage.hpp"
#include <QMutex>

namespace Ugv1
{

extern "C" { UGV1CONTROLLERAGENT_DLL_API IAgent* createAgent(AgentBus& man); }

/// \class ControllerAgent
/// \brief Main robot controller for UGV1
///
/// \todo
/// - Implement health message
/// - Publish health message at a reduced rate
/// - Implement a separate CommandAgent to read Joystick and translate them to control command
class UGV1CONTROLLERAGENT_DLL_API ControllerAgent : public AgentThread
{
public:
    static const int MAX_PERIOD_MS = 100;

public:
    ControllerAgent(AgentBus& man);
    virtual ~ControllerAgent() throw();
    void configure();
    bool isConfigured() { return _isConfigured; }
    bool isRunning() { return QThread::isRunning(); }
    void stop() throw();

private:
    ControllerAgent(const ControllerAgent &);               //!< disable copy
    ControllerAgent &operator=(const ControllerAgent &);    //!< disable assignment
    void run();
    void onJoystick(const lcm::ReceiveBuffer* rBuf, const std::string& channel, const Ugv1Messages::JoyMessage* pMsg);
private:
    int     _periodMs;
    bool    _isConfigured;

    Grape::SerialPort   _serialPort;
    RobotModel          _robotModel;
    QMutex              _modelLock;

    lcm::Subscription*              _pJoySubscription;
    Ugv1Messages::CommandMessage    _commandMsg;
    QMutex                          _commandLock;

    std::string _odometryChannel;
    std::string _joystickChannel;
    std::string _healthChannel;

}; // ControllerAgent


} // Ugv1

#endif // UGV1_CONTROLLERAGENT_H
