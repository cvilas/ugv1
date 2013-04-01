//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.h
// Brief    : UGV1 driver board command messages
//==============================================================================

#ifndef UGV1_IOBOARDCOMMAND_H
#define UGV1_IOBOARDCOMMAND_H

#include "ugv1_common.h"

namespace Ugv1
{

class UGV1_DLL_API IoBoardCommand
{
public:

    enum Command
    {
        SET_SERVOCONTROL_MODE = 0x01,
        SET_DIO_MODE = 0x02,
        WRITE_DIO = 0x03,
        READ_DIO = 0x04,
        WRITE_SERVO = 0x05,
        READ_ANALOG = 0x06,
        SET_RS485_BAUD = 0x07,
        DO_RS485_COMM = 0x08,
        WRITE_I2C = 0x09,
        READ_I2C = 0x0A,
        SET_MOTOR_PARAM = 0x0B,
        WRITE_MOTOR_SPEED = 0x0C,
        READ_MOTOR_SPEED  = 0x0D,
        READ_MOTOR_CURRENT = 0x0E,
        SET_PID_GAINS = 0x0F,
        SET_MOTOR_MODE = 0x10,
        WRITE_MOTOR_POWER = 0x11,
        READ_ENCODERS = 0x12,
        READ_BOARD_VERSION = 0xFF
    };

    IoBoardCommand();
    virtual ~IoBoardCommand();
}; // IoBoardCommand

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_H
