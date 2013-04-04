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
    initialise(SET_DIO_SERVOMODE,1,NULL);
}

//-----------------------------------------------------------------------------
SetDioServoModeCommand::SetDioServoModeCommand(char bitmask)
//-----------------------------------------------------------------------------
    : IoBoardCommand()
{
    initialise(SET_DIO_SERVOMODE, 1, &bitmask);
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

//=============================================================================
WriteServoOutCommand::WriteServoOutCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(WRITE_SERVO, 16, NULL);
}

//-----------------------------------------------------------------------------
void WriteServoOutCommand::setChannel(unsigned int channel, unsigned char position, unsigned char speed)
//-----------------------------------------------------------------------------
{
    if(channel > 7)
    {
        return;
    }
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (2 * channel);

    if(position > 180) position = 180;

    *it = position;
    *(it+1) = speed;

    setCommandModified();
}

//-----------------------------------------------------------------------------
void WriteServoOutCommand::getChannel(unsigned int channel, unsigned char& position, unsigned char& speed)
//-----------------------------------------------------------------------------
{
    if(channel > 7)
    {
        return;
    }
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (2 * channel);
    position = *it;
    speed = *(it+1);
}

} // Ugv1

