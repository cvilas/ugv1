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
#include <string>
#include <vector>

namespace Ugv1
{

extern "C" { UGV1CONTROLLERAGENT_DLL_API IAgent* createAgent(AgentBus& man); }

/// \class ControllerAgent
/// \brief Main robot controller for UGV1
///
class UGV1CONTROLLERAGENT_DLL_API ControllerAgent : public AgentThread
{
public:
    static const int MAX_PERIOD_MS = 100;

public:
    ControllerAgent(AgentBus& man) throw(AgentException);
    virtual ~ControllerAgent();
    void configure() throw(AgentException);
    bool isConfigured();
    bool isRunning();

private:
    ControllerAgent(const ControllerAgent &);               //!< disable copy
    ControllerAgent &operator=(const ControllerAgent &);    //!< disable assignment
    void run();

private:
    int                     _periodMs;
    bool                    _isConfigured;
}; // ControllerAgent


} // Ugv1

#endif // UGV1_CONTROLLERAGENT_H
