//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardMessage_servo.cpp
//==============================================================================

#include "IoBoardMessage_servo.h"

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
unsigned char WriteServoOutCommand::getPosition(unsigned int channel)
//-----------------------------------------------------------------------------
{
    if(channel > 7)
    {
        return 0xFF;
    }
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (2 * channel);
    return *it;
}

//-----------------------------------------------------------------------------
unsigned char WriteServoOutCommand::getSpeed(unsigned int channel)
//-----------------------------------------------------------------------------
{
    if(channel > 7)
    {
        return 0;
    }
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + (2 * channel) + 1;
    return *it;
}

} // Ugv1

