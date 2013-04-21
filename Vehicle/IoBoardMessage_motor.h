//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage_motor.h
// Brief    : UGV1 driver board messages - motor commands
//==============================================================================

#ifndef UGV1_IOBOARDMESSAGE_MOTOR_H
#define UGV1_IOBOARDMESSAGE_MOTOR_H

#include "IoBoardMessage.h"

namespace Ugv1
{

/// \class SetMotorParametersCommand
/// \ingroup comms
/// \brief Sets parameters for DC motor speed control
class UGV1VEHICLELIB_DLL_API SetMotorParametersCommand : public IoBoardCommand
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
class UGV1VEHICLELIB_DLL_API WriteMotorSpeedCommand : public IoBoardCommand
{
public:
    WriteMotorSpeedCommand();

    /// Set speed for the first motor
    /// \param index  Motor num. Valid range is 0 - 1.
    /// \param cmps Motor speed in cm/sec. (+ve for forward, -ve for reverse).
    void setSpeed(unsigned int index, int cmps);

    /// \param index  Motor num. Valid range is 0 - 1.
    /// \return Speed setting for motor
    int getSpeed(unsigned int index);

};

/// \class ReadMotorSpeedCommand
/// \ingroup comms
/// \brief Command to read motor speeds
class UGV1VEHICLELIB_DLL_API ReadMotorSpeedCommand : public IoBoardCommand
{
public:
    ReadMotorSpeedCommand() : IoBoardCommand()
    {
        initialise(READ_MOTOR_SPEED,0,NULL);
    }
};

/// \class ReadMotorSpeedResponse
/// \ingroup comms
/// \brief Response to ReadMotorSpeedCommand
class UGV1VEHICLELIB_DLL_API ReadMotorSpeedResponse : public IoBoardResponse
{
public:
    ReadMotorSpeedResponse() : IoBoardResponse() {}
    size_t getExpectedLength() { return 11; }
    MessageID getExpectedId() { return READ_MOTOR_SPEED; }

    /// Get speed of the specified motor
    /// \param index    Motor index (0 or 1)
    /// \return Motor speed in cm/s
    unsigned short getMotorSpeed(unsigned int index);

};

/// \class ReadMotorCurrentCommand
/// \ingroup comms
/// \brief Command to read motor current consumption
class UGV1VEHICLELIB_DLL_API ReadMotorCurrentCommand : public IoBoardCommand
{
public:
    ReadMotorCurrentCommand() : IoBoardCommand()
    {
        initialise(READ_MOTOR_CURRENT,0,NULL);
    }
};

/// \class ReadMotorCurrentResponse
/// \ingroup comms
/// \brief Response to ReadMotorCurrentCommand
class UGV1VEHICLELIB_DLL_API ReadMotorCurrentResponse : public IoBoardResponse
{
public:
    ReadMotorCurrentResponse() : IoBoardResponse() {}
    size_t getExpectedLength() { return 11;}
    MessageID getExpectedId() { return READ_MOTOR_CURRENT; }

    /// Get motor current consumption
    /// \param index  Motor number (0 or 1)
    /// \return motor current in mA.
    unsigned short getMotorCurrent(unsigned int index);
};

/// \class SetMotorPidGainsCommand
/// \ingroup comms
/// \brief Command to set closed loop motor control gains
class UGV1VEHICLELIB_DLL_API SetMotorPidGainsCommand : public IoBoardCommand
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
class UGV1VEHICLELIB_DLL_API SetMotorDriveModeCommand : public IoBoardCommand
{
public:
    /// Default command sets PID control mode
    SetMotorDriveModeCommand();
    void setModeSpeedControl(); //!< closed loop PID control mode
    void setModeDirectPower();  //!< open loop mode
    bool isModeSpeedControl();
    bool isModeDirectPower();
};

/// \class WriteMotorPowerCommand
/// \ingroup comms
/// \brief Command to directly set motor power percent
/// \see SetMotorDriveModeCommand
class UGV1VEHICLELIB_DLL_API WriteMotorPowerCommand : public IoBoardCommand
{
public:
    WriteMotorPowerCommand();

    /// Set power for motor.
    /// \param index    motor number (0 or 1)
    /// \param percent  Power range. (-100 to +100)
    void setPower(unsigned int index, int percent);

    /// Get power setting for motor.
    /// \param index    motor number (0 or 1)
    /// \return Power setting (-100 to +100)
    int getPower(unsigned int index);
};

/// \class ReadMotorEncodersCommand
/// \ingroup comms
/// \brief command to read motor encoder counts
class UGV1VEHICLELIB_DLL_API ReadMotorEncodersCommand : public IoBoardCommand
{
public:
    ReadMotorEncodersCommand() : IoBoardCommand()
    {
        initialise(READ_MOTOR_ENCODERS,0,NULL);
    }
};

/// \class ReadMotorEncodersResponse
/// \ingroup comms
/// \brief Response to ReadMotorEncodersCommand
class UGV1VEHICLELIB_DLL_API ReadMotorEncodersResponse : public IoBoardResponse
{
public:
    ReadMotorEncodersResponse() : IoBoardResponse() {}
    size_t getExpectedLength() { return 15; }
    MessageID getExpectedId() { return READ_MOTOR_ENCODERS; }

    /// Get encoder count for motor.
    /// \param index    motor number (0 or 1)
    /// \return encoder count
    /// \note Read manual about encoder count and motor direction
    unsigned int getEncoder(unsigned int index);
};

/// \class ResetMotorEncodersCommand
/// \ingroup comms
/// \brief command to reset motor encoder counters
class UGV1VEHICLELIB_DLL_API ResetMotorEncodersCommand : public IoBoardCommand
{
public:
    ResetMotorEncodersCommand() : IoBoardCommand()
    {
        initialise(RESET_MOTOR_ENCODERS,0,NULL);
    }
};


} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_MOTOR_H
