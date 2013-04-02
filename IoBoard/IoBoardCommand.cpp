//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.cpp
//==============================================================================

#include "IoBoardCommand.h"

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
SetDIOServoModeCommand::SetDIOServoModeCommand()
//=============================================================================
{
    createCommand(SET_DIO_SERVOMODE,1,NULL);
}

//-----------------------------------------------------------------------------
SetDIOServoModeCommand::SetDIOServoModeCommand(char bitmask)
//-----------------------------------------------------------------------------
{
    createCommand(SET_DIO_SERVOMODE, 1, &bitmask);
}

//-----------------------------------------------------------------------------
void SetDIOServoModeCommand::setServoMode(unsigned int channel, bool isServo)
//-----------------------------------------------------------------------------
{
    char mask = (1<<channel)&0xFF;
    if( isServo )
    {
        this->operator [](5) |= mask;
    }
    else
    {
        this->operator [](5) &= ~mask;
    }

    /// \todo: mechanism to make this happen automatically
    this->operator [](6) = computeChecksum();
}

//-----------------------------------------------------------------------------
bool SetDIOServoModeCommand::isServoModeSet(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return (this->operator [](5)>>channel)&0x1;
}


} // Ugv1
