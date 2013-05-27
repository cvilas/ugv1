//==============================================================================
// Project  : UGV1
// Module   : AgentThread
// File     : AgentThread.cpp
//==============================================================================

#include "AgentThread.h"
#include "AgentBus.h"

namespace Ugv1
{

//==============================================================================
AgentThread::AgentThread(AgentBus& man) throw(AgentException)
//==============================================================================
    : IAgent(man), QThread(), _exitFlag(true)
{
}

//------------------------------------------------------------------------------
AgentThread::~AgentThread()
//------------------------------------------------------------------------------
{
    stop();
}

//------------------------------------------------------------------------------
bool AgentThread::isExitFlag() const
//------------------------------------------------------------------------------
{
    bool rc;
    _flagLock.lock();
    rc = _exitFlag;
    _flagLock.unlock();
    return rc;
}

//------------------------------------------------------------------------------
void AgentThread::setExitFlag(bool isExit)
//------------------------------------------------------------------------------
{
    _flagLock.lock();
    _exitFlag = isExit;
    _flagLock.unlock();
}

//------------------------------------------------------------------------------
void AgentThread::start() throw(AgentException)
//------------------------------------------------------------------------------
{
    if( !isConfigured() )
    {
        throw AgentException(0, "[AgentThread::start] : Attempted to start without configuring");
    }
    setExitFlag(false);
    QThread::start();
    if( !QThread::isRunning() )
    {
        throw AgentException(0, "[AgentThread::start] : Error starting thread");
    }
}

//------------------------------------------------------------------------------
void AgentThread::stop() throw()
//------------------------------------------------------------------------------
{
    setExitFlag(true);
    QThread::wait();
}

} // namespace Ugv1
