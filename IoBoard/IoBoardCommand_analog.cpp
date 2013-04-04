//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_analog.cpp
//==============================================================================

#include "IoBoardCommand_analog.h"
#include <cstddef>

namespace Ugv1
{

//=============================================================================
ReadAnalogInCommand::ReadAnalogInCommand()
//=============================================================================
    : IoBoardCommand()
{
    createCommand(READ_ANALOG,0,NULL);
}

} //Ugv1

