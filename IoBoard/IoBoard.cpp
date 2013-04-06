//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoard.cpp
//==============================================================================

#include "IoBoard.h"
#include <iostream>

namespace Ugv1
{

//=============================================================================
IoBoard::IoBoard(Grape::IPort& transport)
//=============================================================================
    : _transport(transport), _timeoutMs(-1), _errorCode(0)
{
}

//-----------------------------------------------------------------------------
IoBoard::~IoBoard()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
bool IoBoard::getVersion(ReadBoardVersionResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadBoardVersionCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::getAnalog(ReadAnalogInResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadAnalogInCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::send(const IoBoardCommand& cmd, IoBoardResponse& reply)
//-----------------------------------------------------------------------------
{
    // send the command
    send(cmd);

    // wait for response
    if( !_transport.waitForRead(_timeoutMs) )
    {
        setError(-1) << "[IoBoard::send]: Error in waitForRead";
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
            setError(-1) << "[IoBoard::send]: Error in availableToRead";
            return false;
        }
        Grape::milliSleep(1);
        --nTries;
        if( nTries < 1 )
        {
            setError(-1) << "[IoBoard::send]: Expected " << nToRead << " bytes, got " << nAvailable;
            return false;
        }
    }
    int nRead = _transport.read(reply);
    if( nRead != nToRead )
    {
        setError(-1) << "[IoBoard::send]: Error in read";
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
        setError(-1) << "[IoBoard::send]: Error in waitForWrite";
        return false;
    }

    // write message
    int nToWrite = cmd.size();
    int nWritten = _transport.write(cmd);
    if( nWritten != nToWrite )
    {
        setError(-1) << "[IoBoard::send]: Error in write";
        return false;
    }

    return true;
}

} // Ugv1
