//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_motor.cpp
//==============================================================================

#include "IoBoardCommand_motor.h"
#include <cstddef>
#include<iostream>
namespace Ugv1
{

//=============================================================================
SetMotorParametersCommand::SetMotorParametersCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(SET_MOTOR_PARAM,6,NULL);
    setGearRatio(640);
    setWheelPerimeter(502);
}

//-----------------------------------------------------------------------------
void SetMotorParametersCommand::setEncoderPPR(unsigned short ppr)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    *it = (char)((ppr >> 8) & 0xFF);
    *(it+1) = (char)(ppr & 0xFF);
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned short SetMotorParametersCommand::getEncoderPPR()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned short value = ((((unsigned short)(*it))&0xFF)<<8)
                        + ((unsigned short)(*(it+1))&0xFF);
    return value;
}

//-----------------------------------------------------------------------------
void SetMotorParametersCommand::setGearRatio(unsigned short gr)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;
    *it = (char)((gr >> 8) & 0xFF);
    *(it+1) = (char)(gr & 0xFF);
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned short SetMotorParametersCommand::getGearRatio()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;
    unsigned short value = ((((unsigned short)(*it))&0xFF)<<8)
                        + ((unsigned short)(*(it+1))&0xFF);
    return value;

}

//-----------------------------------------------------------------------------
void SetMotorParametersCommand::setWheelPerimeter(unsigned short mm)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 4;
    *it = (char)((mm >> 8) & 0xFF);
    *(it+1) = (char)(mm & 0xFF);
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned short SetMotorParametersCommand::getWheelPerimeter()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 4;
    unsigned short value = ((((unsigned short)(*it))&0xFF)<<8)
                        + ((unsigned short)(*(it+1))&0xFF);
    return value;
}

//=============================================================================
WriteMotorSpeedCommand::WriteMotorSpeedCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(WRITE_MOTOR_SPEED,5,NULL);
}

//-----------------------------------------------------------------------------
void WriteMotorSpeedCommand::setMotor1Speed(int speed)
//-----------------------------------------------------------------------------
{
    unsigned short cmps = (unsigned short)(abs(speed)&0xFFFF);
    bool fwdDir = (speed > 0);

    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;

    *it = (char)((cmps >> 8) & 0xFF);
    *(it+1) = (char)(cmps & 0xFF);

    // lsb 4 bits of DIR byte set direction for this motor 0x0: forward, 0xF: backward
    if( fwdDir)
    {
        *(it+4) &= 0xF0;
    }
    else
    {
        *(it+4) |= 0x0F;
    }

    setCommandModified();
}

//-----------------------------------------------------------------------------
int WriteMotorSpeedCommand::getMotor1Speed()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    unsigned short cmps = ((((unsigned short)(*it))&0xFF)<<8)
                        + ((unsigned short)(*(it+1))&0xFF);
    char dir = *(it+4) & 0x0F;
    if( dir == 0)
    {
        return cmps;
    }
    else
    {
        return -cmps;
    }
}

//-----------------------------------------------------------------------------
void WriteMotorSpeedCommand::setMotor2Speed(int speed)
//-----------------------------------------------------------------------------
{
    unsigned short cmps = (unsigned short)(abs(speed)&0xFFFF);
    bool fwdDir = (speed > 0);

    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;

    *it = (char)((cmps >> 8) & 0xFF);
    *(it+1) = (char)(cmps & 0xFF);

    // msb 4 bits of DIR byte set direction for this motor 0x0: forward, 0xF: backward
    if( fwdDir)
    {
        *(it+2) &= 0x0F;
    }
    else
    {
        *(it+2) |= 0xF0;
    }

    setCommandModified();

}

//-----------------------------------------------------------------------------
int WriteMotorSpeedCommand::getMotor2Speed()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;
    unsigned short cmps = ((((unsigned short)(*it))&0xFF)<<8)
                        + ((unsigned short)(*(it+1))&0xFF);
    char dir = *(it+2) & 0xF0;
    if( dir == 0)
    {
        return cmps;
    }
    else
    {
        return -cmps;
    }
}

//=============================================================================
SetMotorPidGainsCommand::SetMotorPidGainsCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(SET_MOTOR_PID_GAINS,3,NULL);
}

//-----------------------------------------------------------------------------
void SetMotorPidGainsCommand::setProportionalGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    *it = gain;
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned char SetMotorPidGainsCommand::getProportionalGain()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    return *it;
}

//-----------------------------------------------------------------------------
void SetMotorPidGainsCommand::setIntegralGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 1;
    *it = gain;
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned char SetMotorPidGainsCommand::getIntegralGain()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 1;
    return *it;
}

//-----------------------------------------------------------------------------
void SetMotorPidGainsCommand::setDerivativeGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;
    *it = gain;
    setCommandModified();
}

//-----------------------------------------------------------------------------
unsigned char SetMotorPidGainsCommand::getDerivativeGain()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 2;
    return *it;
}

//=============================================================================
SetMotorDriveModeCommand::SetMotorDriveModeCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(SET_MOTOR_DRIVEMODE,1,NULL);
}

//-----------------------------------------------------------------------------
void SetMotorDriveModeCommand::setModePidControl()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    *it = 0x00;
    setCommandModified();
}

//-----------------------------------------------------------------------------
void SetMotorDriveModeCommand::setModeDirectPower()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    *it = 0x11;
    setCommandModified();
}

//-----------------------------------------------------------------------------
bool SetMotorDriveModeCommand::isModePidControl()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    return (*it == 0x00);
}

//-----------------------------------------------------------------------------
bool SetMotorDriveModeCommand::isModeDirectPower()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    return (*it == 0x11);
}

//=============================================================================
WriteMotorPowerCommand::WriteMotorPowerCommand()
//=============================================================================
    : IoBoardCommand()
{
    initialise(WRITE_MOTOR_POWER,2,NULL);
}

//-----------------------------------------------------------------------------
void WriteMotorPowerCommand::setPower1(int percent)
//-----------------------------------------------------------------------------
{
    if( percent < -100) percent = -100;
    if( percent > 100) percent = 100;

    unsigned char value = (unsigned char)(abs(percent)&0xFF);
    ( percent < 0) ? ( value = 0x64 - value) : (value = 0x64 + value);

    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    *it = value;
    setCommandModified();
}

//-----------------------------------------------------------------------------
void WriteMotorPowerCommand::setPower2(int percent)
//-----------------------------------------------------------------------------
{
    if( percent < -100) percent = -100;
    if( percent > 100) percent = 100;

    unsigned char value = (unsigned char)(abs(percent)&0xFF);
    ( percent < 0) ? ( value = 0x64 - value) : (value = 0x64 + value);

    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 1;
    *it = value;
    setCommandModified();
}

//-----------------------------------------------------------------------------
int WriteMotorPowerCommand::getPower1()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX;
    return (int)(*it&0xFF) - 100;
}

//-----------------------------------------------------------------------------
int WriteMotorPowerCommand::getPower2()
//-----------------------------------------------------------------------------
{
    iterator it = begin() + MESSAGE_PAYLOAD_INDEX + 1;
    return (int)(*it&0xFF) - 100;
}


} //Ugv1
