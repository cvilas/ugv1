//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.h
// Brief    : UGV1 driver board command messages
//==============================================================================

#ifndef UGV1_IOBOARDCOMMAND_H
#define UGV1_IOBOARDCOMMAND_H

#include "IoBoardMessage.h"

namespace Ugv1
{

class UGV1_DLL_API IoBoardCommand : public IoBoardMessage
{
public:

    enum Command
    {
        SET_DIO_SERVOMODE       = 0x01, //!< Configure digital IO pins as servo outputs
        SET_DIO_DIOMODE         = 0x02, //!< Configure digital IO pins as digital inputs or outputs
        WRITE_DIO               = 0x03, //!< Write digital outputs
        READ_DIO                = 0x04, //!< Read digital inputs
        WRITE_SERVO             = 0x05, //!< Write servo output
        READ_ANALOG             = 0x06, //!< Read analog inputs
        SET_RS485_BAUD          = 0x07, //!< Configure RS485 baud rate
        DO_RS485_COMM           = 0x08, //!< Send data to devices on the RS485 bus
        WRITE_I2C               = 0x09, //!< Write to a device on the i2c bus
        READ_I2C                = 0x0A, //!< Read from a device on the i2c bus
        SET_MOTOR_PARAM         = 0x0B, //!< Configure motor drive-train parameters
        WRITE_MOTOR_SPEED       = 0x0C, //!< Set motor speed and direction
        READ_MOTOR_SPEED        = 0x0D, //!< Get motor speed
        READ_MOTOR_CURRENT      = 0x0E, //!< Get motor currents
        SET_PID_GAINS           = 0x0F, //!< Configure motor controller PID gains
        SET_MOTOR_DRIVEMODE     = 0x10, //!< Set direct drive or PID mode for motor controller
        WRITE_MOTOR_POWER       = 0x11, //!< Set motor power
        READ_ENCODERS           = 0x12, //!< Read motor drive encoders
        READ_BOARD_VERSION      = 0xFF  //!< Get version information
    };

protected:
    IoBoardCommand(int length = -1);
    void assignHeader();
    virtual ~IoBoardCommand();

}; // IoBoardCommand


class UGV1_DLL_API SetServoModeCommand : public IoBoardCommand
{
public:
    SetServoModeCommand(char bitmask) : IoBoardCommand(7)
    {
        std::vector<char>::push_back(0x01);             // length of payload
        std::vector<char>::push_back(SET_DIO_DIOMODE);  // command type
        std::vector<char>::push_back(bitmask);          // payload
        addChecksum();
    }
}; // SetServoModeCommand

class UGV1_DLL_API SetDioMode : public IoBoardCommand
{
public:
    SetDioMode(char highIO, char lowIO) : IoBoardCommand(8)
    {
        push_back(0x02);
        push_back(SET_DIO_DIOMODE);
        push_back(highIO);
        push_back(lowIO);
        addChecksum();
    }
}; // SetDioMode

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_H
