//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_i2c.h
// Brief    : UGV1 driver board command messages - motor commands
//==============================================================================

#ifndef UGV1_IOBOARDCOMMAND_MOTOR_H
#define UGV1_IOBOARDCOMMAND_MOTOR_H

#include "IoBoardCommand.h"

namespace Ugv1
{

/// \class SetMotorParametersCommand
/// \ingroup comms
/// \brief Sets parameters for DC motor speed control
class UGV1_DLL_API SetMotorParametersCommand : public IoBoardCommand
{
public:

    /// Create. Defaults are
    /// - encoder ppr = 12
    /// - gear ratio = 64:1
    /// - wheel perimeter = 502 mm
    SetMotorParametersCommand();

    /// Set encoder pulses per rotation.
    void setEncoderPPR(unsigned short ppr);
    unsigned short getEncoderPPR();

    /// Set drive train gear ratio.
    /// \param gr   Gear ratio multiplied by 10. i.e. If the actual ratio is
    ///             64:1, set 640.
    void setGearRatio(unsigned short gr);

    /// \see setGearRatio
    unsigned short getGearRatio();

    /// Set the wheel perimeter
    /// \param mm   Wheel perimeter in millimeters.
    void setWheelPerimeter(unsigned short mm);

    /// \see setWheelPerimeter
    unsigned short getWheelPerimeter();
};

/// \class WriteMotorSpeedCommand
/// \ingroup comms
/// \brief command to set motor speeds
/// The motor constroller mode must be set to PID control for this to work
/// \see SetMotorDriveModeCommand
class UGV1_DLL_API WriteMotorSpeedCommand : public IoBoardCommand
{
public:
    WriteMotorSpeedCommand();

    /// Set speed for the first motor
    /// \param cmps     Motor speed in cm/sec. (+ve for forward, -ve for reverse).
    void setMotor1Speed(int speed);

    /// \return Speed setting for motor 1
    int getMotor1Speed();

    /// Set speed for the second motor
    /// \param cmps     Motor speed in cm/sec. (+ve for forward, -ve for reverse).
    void setMotor2Speed(int speed);

    /// \return Speed setting for motor 2
    int getMotor2Speed();
};

/// \class ReadMotorSpeedCommand
/// \ingroup comms
/// \brief Command to read motor speeds
class UGV1_DLL_API ReadMotorSpeedCommand : public IoBoardCommand
{
public:
    ReadMotorSpeedCommand() : IoBoardCommand()
    {
        createCommand(READ_MOTOR_SPEED,0,NULL);
    }
};

/// \class ReadMotorCurrentCommand
/// \ingroup comms
/// \brief Command to read motor current consumption
class UGV1_DLL_API ReadMotorCurrentCommand : public IoBoardCommand
{
public:
    ReadMotorCurrentCommand() : IoBoardCommand()
    {
        createCommand(READ_MOTOR_CURRENT,0,NULL);
    }
};

/// \class SetMotorPidGainsCommand
/// \ingroup comms
/// \brief Command to set closed loop motor control gains
class UGV1_DLL_API SetMotorPidGainsCommand : public IoBoardCommand
{
public:
    SetMotorPidGainsCommand();

    /// Set PID proportional gain
    /// \param gain    Proportional gain multiplied by 10. Range 0 - 255.
    void setProportionalGain(unsigned char gain);
    unsigned char getProportionalGain();

    /// Set PID integral gain
    /// \param gain    Integral gain multiplied by 10. Range 0 - 255.
    void setIntegralGain(unsigned char gain);
    unsigned char getIntegralGain();

    /// Set PID derivative gain
    /// \param gain    Derivative gain multiplied by 10. Range 0 - 255.
    void setDerivativeGain(unsigned char gain);
    unsigned char getDerivativeGain();
};

/// \class SetMotorDriveModeCommand
/// \ingroup comms
/// \brief Command to set motor control in open or closed loop mode
/// \see WriteMotorPowerCommand, WriteMotorSpeedCommand
class UGV1_DLL_API SetMotorDriveModeCommand : public IoBoardCommand
{
public:
    /// Default command sets PID control mode
    SetMotorDriveModeCommand();
    void setModePidControl();   //!< closed loop PID control mode
    void setModeDirectPower();  //!< open loop mode
    bool isModePidControl();
    bool isModeDirectPower();
};

/// \class WriteMotorPowerCommand
/// \ingroup comms
/// \brief Command to directly set motor power percent
/// \see SetMotorDriveModeCommand
class UGV1_DLL_API WriteMotorPowerCommand : public IoBoardCommand
{
public:
    WriteMotorPowerCommand();

    /// Set power for first motor. Range -100 to +100
    void setPower1(int percent);

    /// Set power for second motor. Range -100 to +100.
    void setPower2(int percent);

    int getPower1();
    int getPower2();
};

/// \class ReadMotorEncodersCommand
/// \ingroup comms
/// \brief command to read motor encoder counts
class UGV1_DLL_API ReadMotorEncodersCommand : public IoBoardCommand
{
public:
    ReadMotorEncodersCommand() : IoBoardCommand()
    {
        createCommand(READ_MOTOR_ENCODERS,0,NULL);
    }
};

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_MOTOR_H
