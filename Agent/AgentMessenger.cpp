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
    if( !start() )
    {
        throw MessengerException(0, "[AgentMessenger] Message handler did not start");
    }
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
bool AgentMessenger::start()
//------------------------------------------------------------------------------
{
    setExitFlag(false);
    QThread::start();
    return QThread::isRunning();
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
    qDebug() << "[AgentMessenger::run] Entered";

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
            qDebug() << "[AgentMessenger::run] Waiting for message";
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

    qDebug() << "[AgentMessenger::run] Exiting";
}

} // Ugv1
