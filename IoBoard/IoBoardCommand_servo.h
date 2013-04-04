//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_servo.h
// Brief    : UGV1 driver board command messages - servo outputs
//==============================================================================

#ifndef UGV1_IOBOARDCOMMAND_SERVO_H
#define UGV1_IOBOARDCOMMAND_SERVO_H

#include "IoBoardCommand.h"

namespace Ugv1
{

/// \class SetDIOServoModeCommand
/// \ingroup comms
/// \brief Command to configure digital outputs as PWM servo drives
class UGV1_DLL_API SetDioServoModeCommand : public IoBoardCommand
{
public:

    /// Default constructor. Doesn't set any digital output as PWM drive.
    SetDioServoModeCommand();

    /// Construct the command with digital outputs configured as PWM drives
    /// \param bitmask 8 bit mask of digital IO0 (lsb) - IO7 (msb) to set as PWM servo
    ///                 output (1) or not (0)
    SetDioServoModeCommand(char bitmask);

    /// Set a specific channel (0 - 7) as PWM servo output
    /// \param channel  Channel number 0 - 7 for digital IO0-IO7
    /// \param option   Set this to true to set as PWM output.
    void setModeServo(unsigned int channel, bool option);

    /// \return true if the specified channel is set to be PWM servo output
    bool isModeServo(unsigned int channel);

}; // SetDIOServoModeCommand

/// \class WriteServoOutCommand
/// \ingroup comms
/// \brief Command to set servo output position and speed.
/// After configuring the DIO0-7 to Servo control mode, this command is used to play the Servo
/// with position and speed control.
class UGV1_DLL_API WriteServoOutCommand : public IoBoardCommand
{
public:
    /// Default constructor creates a command with all positions and speeds set to 0.
    WriteServoOutCommand();

    /// Set position and speed for a servo channel
    /// \param channel  Servo channel in range 0 - 7.
    /// \param position Position value of the servo in range 0 to 180. Center position is 90.
    /// \param speed    Speed value of the servo from 0 to 0xFF.
    void setChannel(unsigned int channel, unsigned char position, unsigned char speed);

    /// Get the position and speed setting for a servo channel
    /// \param channel  Servo channel in range 0 - 7. Method returns doing nothing if channel is invalid.
    void getChannel(unsigned int channel, unsigned char& position, unsigned char& speed);

}; // WriteServoOutCommand

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_SERVO_H
