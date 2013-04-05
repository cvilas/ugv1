//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage_analog.cpp
//==============================================================================

#include "IoBoardMessage_analog.h"
#include <cstddef>

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
unsigned short ReadAnalogInResponse::getChannel(unsigned int channel)
//=============================================================================
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (channel * 2);
    unsigned short value = ((((unsigned short)(*it))&0x0F)<<8) + ((unsigned short)(*(it+1))&0xFF);
    return value;
}

} //Ugv1

