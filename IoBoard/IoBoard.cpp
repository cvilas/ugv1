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
    : _transport(transport), _timeoutMs(-1)
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
bool IoBoard::getDigitalIn(ReadDioInResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadDioInCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::getMotorSpeed(ReadMotorSpeedResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorSpeedCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::getMotorCurrent(ReadMotorCurrentResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorCurrentCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::getMotorEncoders(ReadMotorEncodersResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorEncodersCommand cmd;
    return send(cmd, response);
}

//-----------------------------------------------------------------------------
bool IoBoard::resetMotorEncoders()
//-----------------------------------------------------------------------------
{
    Ugv1::ResetMotorEncodersCommand cmd;
    return send(cmd);
}

//-----------------------------------------------------------------------------
bool IoBoard::send(const IoBoardCommand& cmd, IoBoardResponse& reply)
//-----------------------------------------------------------------------------
{
    // send the command
    if( !send(cmd) )
    {
        return false;
    }

    // wait for response
    if( !_transport.waitForRead(_timeoutMs) )
    {
        lastError.set(-1) << "[IoBoard::send]: Error in waitForRead";
        return false;
    }

    // ok. we got something in the read buffer. now wait
    // until the entire message is available
    int nToRead = reply.getExpectedLength();
/*
    // ** Sensor/Motor board v1.2.2 firmware bug workaround **
    // -> reply does not contain ID. Expect one less byte to read
    if( cmd.getId() == IoBoardMessage::READ_MOTOR_SPEED)
    {
        nToRead -= 1;
    }
*/
    int nTries = 10;
    int nAvailable = 0;
    while( (nAvailable < nToRead) && (nTries > 0) )
    {
        nAvailable = _transport.availableToRead();
        if( nAvailable < 0 )
        {
            lastError.set(-1) << "[IoBoard::send]: Error in availableToRead";
            return false;
        }
        Grape::milliSleep(1);
        --nTries;
        if( nTries < 1 )
        {
            lastError.set(-1) << "[IoBoard::send]: Expected " << nToRead << " bytes, " << nAvailable << " available";
            return false;
        }
    }
    int nRead = _transport.read(reply);
    if( nRead != nToRead )
    {
        lastError.set(-1) << "[IoBoard::send]: Error in read. Read " << nRead << "/" << nToRead;
        return false;
    }
/*
    // ** Sensor/Motor board v1.2.2 firmware bug workaround **
    // -> reply does not contain ID. insert it
    if( cmd.getId() == IoBoardMessage::READ_MOTOR_SPEED)
    {
        reply.insert(reply.begin() + IoBoardMessage::MESSAGE_ID_INDEX, 1, cmd.getId() );
    }
*/
    return true;
}

//-----------------------------------------------------------------------------
bool IoBoard::send(const IoBoardCommand& cmd)
//-----------------------------------------------------------------------------
{
    // write message
    int nToWrite = cmd.size();
    int nWritten = _transport.write(cmd);
    if( nWritten != nToWrite )
    {
        lastError.set(-1) << "[IoBoard::send]: Error in write. Wrote " << nWritten << "/" << nToWrite;
        return false;
    }

    // wait for write operation to complete
    if( !_transport.waitForWrite(-1) )
    {
        lastError.set(-1) << "[IoBoard::send]: Error in waitForWrite";
        return false;
    }

    return true;
}

} // Ugv1
