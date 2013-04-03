//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.cpp
//==============================================================================

#include "IoBoardCommand.h"
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
void SetDioServoModeCommand::setServoMode(unsigned int channel, bool setServo)
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
bool SetDioServoModeCommand::isServoModeSet(unsigned int channel)
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
    char bm[2] = { (char)(bitmask&0xFF), (char)((bitmask>>8)&0x7)};
    createCommand(SET_DIO_IOMODE,2, bm);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setInputMode(int channel, bool setInput)
//-----------------------------------------------------------------------------
{
    setOutputMode(channel, !setInput);
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isInputModeSet(int channel)
//-----------------------------------------------------------------------------
{
    return !isOutputModeSet(channel);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setOutputMode(int channel, bool setOutput)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;

    // todo

    setCommandModified();
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isOutputModeSet(int channel)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;

    // todo

    //return (bm>>channel)&0x1;
}


} // Ugv1
