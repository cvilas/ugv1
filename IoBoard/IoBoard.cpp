//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoard.cpp
//==============================================================================

#include "IoBoard.h"

namespace Ugv1
{

//=============================================================================
IoBoard::IoBoard(Grape::IPort& transport)
//=============================================================================
    : _transport(transport), _timeoutMs(-1)
{
}

//-----------------------------------------------------------------------------
IoBoard::~IoBoard()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
bool IoBoard::send(const IoBoardCommand& cmd, IoBoardResponse& reply)
//-----------------------------------------------------------------------------
{
    // send the command
    send(cmd);

    // wait for response
    if( !_transport.waitForRead(_timeoutMs) )
    {
        /// \todo set error message
        return false;
    }

    // ok. we got something in the read buffer. now wait
    // until the entire message is available
    int nToRead = reply.getExpectedLength();
    int nTries = 10;
    int nAvailable = 0;
    while( (nAvailable < nToRead) && (nTries > 0) )
    {
        nAvailable = _transport.availableToRead();
        if( nAvailable < 0 )
        {
            /// \todo set error message
            return false;
        }
        Grape::milliSleep(1);
        --nTries;
        if( nTries < 1 )
        {
            /// \todo error message
            return false;
        }
    }
    int nRead = _transport.read(reply);
    if( nRead != nToRead )
    {
        /// \todo error message
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool IoBoard::send(const IoBoardCommand& cmd)
//-----------------------------------------------------------------------------
{
    // wait for hardware to be ready
    if( !_transport.waitForWrite(-1) )
    {
        /// \todo set error message
        return false;
    }

    // write message
    int nToWrite = cmd.size();
    int nWritten = _transport.write(cmd);
    if( nWritten != nToWrite )
    {
        /// \todo set error message
        return false;
    }

    return true;
}

} // Ugv1
