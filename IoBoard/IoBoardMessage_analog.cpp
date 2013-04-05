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

} //Ugv1

