//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_servo.cpp
//==============================================================================

#include "IoBoardCommand_servo.h"
#include <cstddef>

namespace Ugv1
{

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


} // Ugv1

