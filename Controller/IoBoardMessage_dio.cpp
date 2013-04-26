//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardMessage_dio.cpp
//==============================================================================

#include "IoBoardMessage_dio.h"

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
void SetDioIoModeCommand::setModeOutput(unsigned int channel, bool isTrue)
//-----------------------------------------------------------------------------
{
    setModeInput(channel, !isTrue);
}

//-----------------------------------------------------------------------------
bool SetDioIoModeCommand::isModeOutput(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return !isModeInput(channel);
}

//-----------------------------------------------------------------------------
void SetDioIoModeCommand::setModeInput(unsigned int channel, bool isTrue)
//-----------------------------------------------------------------------------
{
    /// \note: ** According to the manual, to configure a digital pin as
    /// output, the corresponding bit must be set to 1. However, in practise
    /// it was found that setting the bit to 1 configures the pin as input

    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned int bm = ((((unsigned int)(*it))&0x7)<<8) + ((unsigned int)(*(it+1))&0xFF);
    unsigned int mask = (1<<channel);

    if(isTrue)
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
bool SetDioIoModeCommand::isModeInput(unsigned int channel)
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

