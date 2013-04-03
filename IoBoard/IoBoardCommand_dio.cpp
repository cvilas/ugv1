//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_dio.cpp
//==============================================================================

#include "IoBoardCommand_dio.h"
#include <cstddef>

namespace Ugv1
{

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

//=============================================================================
WriteDioOutCommand::WriteDioOutCommand()
//=============================================================================
{}

//-----------------------------------------------------------------------------
WriteDioOutCommand::WriteDioOutCommand(int bitmask)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void WriteDioOutCommand::setChannel(int channel, bool high)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
bool WriteDioOutCommand::getChannel(int channel)
//-----------------------------------------------------------------------------
{
    return false;
}

} // Ugv1

