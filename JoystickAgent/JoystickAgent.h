//==============================================================================
// Project  : UGV1
// Module   : JoystickAgent
// File     : JoystickAgent.h
// Brief    : Publishes joystick messages
//==============================================================================

#ifndef UGV1_JOYSTICKAGENT_H
#define	UGV1_JOYSTICKAGENT_H

#ifdef UGV1JOYSTICKAGENT_DLL
#	ifdef UGV1JOYSTICKAGENT_DLL_EXPORT
#		define UGV1JOYSTICKAGENT_DLL_API __declspec(dllexport)
#	else
#		define UGV1JOYSTICKAGENT_DLL_API __declspec(dllimport)
#	endif
#else
#	define UGV1JOYSTICKAGENT_DLL_API
#endif


#include "IAgent.h"
#include <QThread>
#include <QMutex>
#include <string>
#include <vector>

namespace Grape { class SimpleJoystick; }

namespace Ugv1
{

extern "C" { UGV1JOYSTICKAGENT_DLL_API IAgent* createAgent(AgentBus& man); }

/// \class JoyCtrl
/// \brief Joystick input control mapping
class UGV1JOYSTICKAGENT_DLL_API JoyCtrl
{
public:
    enum Type { AXIS, POV, BUTTON, Type_MAX };
    static Type TypeFromString(std::string s);

public:
    JoyCtrl();
    virtual ~JoyCtrl(){}

    Type type() const { return _type; }

    void invalidate() { _type = Type_MAX; }
    bool isValid() const { return (_type != Type_MAX); }

    /// Attach a control input.
    /// \param type
    /// \param index
    /// \param scale
    /// \param js
    void attach (const std::string& type, unsigned int index, int scale, const Grape::SimpleJoystick& js) throw(AgentException);

    /// read state of the mapped input control
    int value() { return (isValid()?((*_pCtrl) * _scale):(0)); }

private:
    int                     _scale;
    Type                    _type;
    std::vector<int>::const_iterator _pCtrl;
};

/// \class JoystickAgent
/// \brief Mapped joystick inputs for UGV1
///
/// Joystick agent publishes raw joystick messages after mapping axes and buttons
/// to UGV1 controls. See JoyMessage.lcm for a description of elements in the
/// message data structure.
///
/// Example program:
/// \include JoystickSubscriber.cpp
class UGV1JOYSTICKAGENT_DLL_API JoystickAgent : public IAgent, public QThread
{
public:
    static const int MAX_PERIOD_MS = 100;

public:
    JoystickAgent(AgentBus& man) throw(AgentException);
    virtual ~JoystickAgent();
    void configure() throw(AgentException);
    void start() throw(AgentException);
    void stop() throw();

private:
    JoystickAgent(const JoystickAgent &);               //!< disable copy
    JoystickAgent &operator=(const JoystickAgent &);    //!< disable assignment
    void run();     //!< The handler thread function
    bool isExitFlag() const;
    void setExitFlag(bool isExit);
private:
    int                     _periodMs;
    bool                    _exitFlag;
    mutable QMutex          _flagLock;
    bool                    _isConfigured;
    std::string             _lcmChannel;//!< publish on this channel
    Grape::SimpleJoystick*  _pJoystick;     //!< device
    std::string             _jsPort;

    JoyCtrl _surgeCtrl;
    JoyCtrl _yawCtrl;
    JoyCtrl _deadMansCtrl;

}; // JoystickAgent


} // Ugv1

#endif // UGV1_JOYSTICKAGENT_H
