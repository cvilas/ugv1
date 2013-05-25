//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentMessenger.cpp
//==============================================================================

#include "AgentMessenger.h"
#include <QDebug>

namespace Ugv1
{

//==============================================================================
AgentMessenger::AgentMessenger(const std::string& url) throw(AgentException)
//==============================================================================
    : QThread(), _exitFlag(false), _lcm(url)
{
    start();
}

//------------------------------------------------------------------------------
AgentMessenger::~AgentMessenger()
//------------------------------------------------------------------------------
{
    stop();
}

//------------------------------------------------------------------------------
bool AgentMessenger::isGood() const
//------------------------------------------------------------------------------
{
    return _lcm.good();
}

//------------------------------------------------------------------------------
bool AgentMessenger::isExitFlag() const
//------------------------------------------------------------------------------
{
    bool rc;
    _flagLock.lock();
    rc = _exitFlag;
    _flagLock.unlock();
    return rc;
}

//------------------------------------------------------------------------------
void AgentMessenger::setExitFlag(bool isExit)
//------------------------------------------------------------------------------
{
    _flagLock.lock();
    _exitFlag = isExit;
    _flagLock.unlock();
}

//------------------------------------------------------------------------------
void AgentMessenger::start() throw(AgentException)
//------------------------------------------------------------------------------
{
    setExitFlag(false);
    QThread::start();
    if( !QThread::isRunning() )
    {
        throw MessengerException(0, "[AgentMessenger::start] Thread didn't start");
    }

}

//------------------------------------------------------------------------------
void AgentMessenger::stop()
//------------------------------------------------------------------------------
{
    setExitFlag(true);
    QThread::wait();
}

//------------------------------------------------------------------------------
void AgentMessenger::run()
//------------------------------------------------------------------------------
{
#ifdef _DEBUG
    qDebug() << "[AgentMessenger::run] Entered";
#endif
    while( !isExitFlag() )
    {
        // use file descriptor here rather than wait(), because we want to
        // wait with a timeout

        int fd = _lcm.getFileno();
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        struct timeval timeout =
        {
            1,  // seconds
            0   // microseconds
        };

        int status = select(fd + 1, &fds, 0, 0, &timeout);

        if(0 == status)
        {
            // no messages
#ifdef _DEBUG
            qDebug() << "[AgentMessenger::run] Waiting for message";
#endif
        }
        else if( FD_ISSET(fd, &fds) )
        {
            // LCM message pending
            if( -1 == _lcm.handle() )
            {
                throw MessengerException(0, "[AgentMessenger::run] : LCM handler error");
            }
        }
    }
#ifdef _DEBUG
    qDebug() << "[AgentMessenger::run] Exiting";
#endif
}

} // Ugv1
