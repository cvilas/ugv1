//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage_dio.cpp
//==============================================================================

#include "IoBoardMessage_dio.h"
#include <cstddef>

namespace Ugv1
{

//=============================================================================
SetDioIoModeCommand::SetDioIoModeCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(SET_DIO_IOMODE,2,NULL);
}

//-----------------------------------------------------------------------------
SetDioIoModeCommand::SetDioIoModeCommand(int bitmask)
//-----------------------------------------------------------------------------
{
    char bm[2] = { (char)((bitmask>>8)&0x7), (char)(bitmask&0xFF)};
    initialise(SET_DIO_IOMODE,2, bm);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setModeInput(unsigned int channel, bool setInput)
//-----------------------------------------------------------------------------
{
    setModeOutput(channel, !setInput);
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isModeInput(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return !isModeOutput(channel);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setModeOutput(unsigned int channel, bool setOutput)
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
bool SetDioIoModeCommand::isModeOutput(unsigned int channel)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    return (bm>>channel)&0x1;
}

//=============================================================================
WriteDioOutCommand::WriteDioOutCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(WRITE_DIO,2,NULL);
}

//-----------------------------------------------------------------------------
WriteDioOutCommand::WriteDioOutCommand(int bitmask)
//-----------------------------------------------------------------------------
{
    char bm[2] = { (char)((bitmask>>8)&0x7), (char)(bitmask&0xFF)};
    initialise(WRITE_DIO,2,bm);
}

//-----------------------------------------------------------------------------
void WriteDioOutCommand::setChannel(unsigned int channel, bool high)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    unsigned int mask = (1<<channel);

    if(high)
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
bool WriteDioOutCommand::getChannel(unsigned int channel)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    return (bm>>channel)&0x1;
}

//=============================================================================
ReadDioInCommand::ReadDioInCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(READ_DIO,0,NULL);
}


//=============================================================================
bool ReadDioInResponse::isHigh(unsigned int channel)
//=============================================================================
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    return (bm>>channel)&0x1;
}


} // Ugv1

