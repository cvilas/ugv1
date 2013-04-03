//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.cpp
//==============================================================================

#include "IoBoardCommand_servo.h"
#include "IoBoardCommand_dio.h"
#include <cstddef>

namespace Ugv1
{

//=============================================================================
void IoBoardCommand::createCommand(Command cmd, int nPayloadBytes, char* payload)
//=============================================================================
{
    // message format: [header(3)][payload length(1)][cmd(1)][payload(n)][checksum(1)]

    resize(nPayloadBytes + 6);
    std::vector<char>::iterator it = begin();
    *it = IoBoardMessage::MESSAGE_HEADER[0]; ++it;
    *it = IoBoardMessage::MESSAGE_HEADER[1]; ++it;
    *it = IoBoardMessage::MESSAGE_HEADER[2]; ++it;
    *it = nPayloadBytes; ++it;
    *it = cmd; ++it;
    for( int i = 0; i < nPayloadBytes; ++i)
    {
        (payload != NULL) ? (*it = payload[i]) : (*it = 0);
        ++it;
    }
    *it = computeChecksum();
}

//-----------------------------------------------------------------------------
void IoBoardCommand::setCommandModified()
//-----------------------------------------------------------------------------
{
    *(end()-1) = computeChecksum();
}

//-----------------------------------------------------------------------------
char IoBoardCommand::computeChecksum()
//-----------------------------------------------------------------------------
{
    unsigned int csum = 0;
    const_iterator it = begin();
    const_iterator itEnd = end() - 1; // checksum is the last byte
    while( it != itEnd )
    {
        csum += (unsigned int)(*it);
        ++it;
    }
    return( (char)(csum&0xFF));
}

//=============================================================================
SetDioServoModeCommand::SetDioServoModeCommand()
//=============================================================================
    : IoBoardCommand()
{
    createCommand(SET_DIO_SERVOMODE,1,NULL);
}

//-----------------------------------------------------------------------------
SetDioServoModeCommand::SetDioServoModeCommand(char bitmask)
//-----------------------------------------------------------------------------
    : IoBoardCommand()
{
    createCommand(SET_DIO_SERVOMODE, 1, &bitmask);
}

//-----------------------------------------------------------------------------
void SetDioServoModeCommand::setModeServo(unsigned int channel, bool setServo)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    char mask = (1<<channel)&0xFF;
    if( setServo )
    {
        *it |= mask;
    }
    else
    {
        *it &= ~mask;
    }

    setCommandModified();
}

//-----------------------------------------------------------------------------
bool SetDioServoModeCommand::isModeServo(unsigned int channel)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    return ((*it)>>channel)&0x1;
}

//=============================================================================
SetDioIoModeCommand::SetDioIoModeCommand()
//=============================================================================
    : IoBoardCommand()
{
    createCommand(SET_DIO_IOMODE,2,NULL);
}

//-----------------------------------------------------------------------------
SetDioIoModeCommand::SetDioIoModeCommand(int bitmask)
//-----------------------------------------------------------------------------
{
    char bm[2] = { (char)((bitmask>>8)&0x7), (char)(bitmask&0xFF)};
    createCommand(SET_DIO_IOMODE,2, bm);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setModeInput(int channel, bool setInput)
//-----------------------------------------------------------------------------
{
    setModeOutput(channel, !setInput);
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isModeInput(int channel)
//-----------------------------------------------------------------------------
{
    return !isModeOutput(channel);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setModeOutput(int channel, bool setOutput)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    unsigned int mask = (1<<channel);

    if(setOutput)
    {
        bm |= mask;
    }
    else
    {
        bm &= ~mask;
    }

    *it = (char)((bm>>8)&0x7);
    *(it+1) = (char)(bm&0xFF);

    setCommandModified();
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isModeOutput(int channel)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    return (bm>>channel)&0x1;
}


} // Ugv1
