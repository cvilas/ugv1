//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentThread.h
// Brief    : Thread wrapper for Agents
//==============================================================================

#ifndef UGV1_AGENTTHREAD_H
#define	UGV1_AGENTTHREAD_H

#include "IAgent.h"
#include <QThread>
#include <QMutex>

namespace Ugv1
{

/// \class AgentThread
/// \brief Thread wrapper for agents
class UGV1AGENTLIB_DLL_API AgentThread : public IAgent, public QThread
{
public:
    AgentThread(AgentBus& man) throw(AgentException);
    virtual ~AgentThread();

    void start() throw(AgentException); //!< Run run() in separate thread
    void stop() throw();                //!< Set flag for run() to exit

protected:
    virtual void run() = 0;     //!< The handler thread function. Must check isExitFlag() periodically to test whether it must exit.
    bool isExitFlag() const;
    void setExitFlag(bool isExit);
private:
    AgentThread(const AgentThread &);               //!< disable copy
    AgentThread &operator=(const AgentThread &);    //!< disable assignment
private:
    bool                    _exitFlag;
    mutable QMutex          _flagLock;
}; // AgentThread


} // Ugv1

#endif // UGV1_AGENTTHREAD_H
