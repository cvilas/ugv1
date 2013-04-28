//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardMessage_analog.cpp
//==============================================================================

#include "IoBoardMessage_analog.h"

namespace Ugv1
{

//=============================================================================
ReadAnalogInCommand::ReadAnalogInCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(READ_ANALOG,0,NULL);
}

//=============================================================================
unsigned short ReadAnalogInResponse::getCount(unsigned int channel)
//=============================================================================
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (channel * 2);
    unsigned short value = ((((unsigned short)(*it))&0x0F)<<8) + ((unsigned short)(*(it+1))&0xFF);
    return value;
}

//-----------------------------------------------------------------------------
double ReadAnalogInResponse::getVolts(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return 3.3 * getCount(channel) / (double)0xfff;
}

} //Ugv1

