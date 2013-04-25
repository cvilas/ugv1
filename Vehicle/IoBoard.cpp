//==============================================================================
// Project  : UGV1
// Module   : Vehicle
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
void IoBoard::getVersion(ReadBoardVersionResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadBoardVersionCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::getAnalog(ReadAnalogInResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadAnalogInCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::getDigitalIn(ReadDioInResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadDioInCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::getMotorSpeed(ReadMotorSpeedResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorSpeedCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::getMotorCurrent(ReadMotorCurrentResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorCurrentCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::getMotorEncoders(ReadMotorEncodersResponse& response)
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorEncodersCommand cmd;
    send(cmd, response);
}

//-----------------------------------------------------------------------------
void IoBoard::resetMotorEncoders()
//-----------------------------------------------------------------------------
{
    Ugv1::ResetMotorEncodersCommand cmd;
    send(cmd);
}

//-----------------------------------------------------------------------------
void IoBoard::send(const IoBoardCommand& cmd, IoBoardResponse& reply) throw(VehicleException)
//-----------------------------------------------------------------------------
{
    send(cmd);

    // wait for response
    if( _transport.waitForRead(_timeoutMs) <= 0 )
    {
		_transport.flushRx();

		std::ostringstream str;
		str << __FUNCTION__ << ": Error or timeout waiting for response to message ID " 
			<< cmd.getIdFromMessage() << ".";
        throw IoWaitException(0, str.str());
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
			_transport.flushRx();

			std::ostringstream str;
			str << __FUNCTION__ << ": For message ID " << cmd.getIdFromMessage() 
				<< " error checking read buffer for bytes available.";
            throw IoReadException(0, str.str());
        }
        Grape::milliSleep(1);
        --nTries;
        if( nTries < 1 )
        {
            _transport.flushRx();

			std::ostringstream str;
			str << __FUNCTION__ << ": For message ID " << cmd.getIdFromMessage() 
				<< ", full response not available after additional wait. Expected " << nToRead 
				<< " bytes, got only " << nAvailable << ".";
            throw IoWaitException(0, str.str());
        }
    }
    int nRead = _transport.read(reply);
    if( nRead != nToRead )
    {
        _transport.flushRx();

		std::ostringstream str;
		str << __FUNCTION__ << ": For message ID " << cmd.getIdFromMessage() 
			<< ", read " << nRead << "/" << nToRead << " bytes.";
        throw IoReadException(0, str.str());
    }

	// validate
	reply.validate();
}

//-----------------------------------------------------------------------------
void IoBoard::send(const IoBoardCommand& cmd) throw (VehicleException)
//-----------------------------------------------------------------------------
{
    // write message
    int nToWrite = cmd.size();
    int nWritten = _transport.write(cmd);
    if( nWritten != nToWrite )
    {
        _transport.flushTx();

		std::ostringstream str;
		str << __FUNCTION__ << ": For message ID " << cmd.getIdFromMessage() 
			<< ", wrote " << nWritten << "/" << nToWrite << " bytes.";
        throw IoWriteException(0, str.str());
    }

    // wait for write operation to complete
    if( _transport.waitForWrite(-1) <= 0 )
    {
		_transport.flushTx();

		std::ostringstream str;
		str << __FUNCTION__ << ": Write operation didn't complete For message ID " 
			<< cmd.getIdFromMessage() << ".";
        throw IoWaitException(0, str.str());
    }
}

} // Ugv1
