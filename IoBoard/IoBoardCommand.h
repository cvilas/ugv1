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

/// \class IoBoardCommand
/// \ingroup comms
/// \brief IO board command message builder. Base class for all IoBoard commands.
/// \see IoBoardReply, IoBoardMessage
class UGV1_DLL_API IoBoardCommand : public IoBoardMessage
{
public:
    enum Command
    {
        SET_DIO_SERVOMODE       = 0x01, //!< Configure digital IO pins as servo outputs
        SET_DIO_IOMODE          = 0x02, //!< Configure digital IO pins as digital inputs or outputs
        WRITE_DIO               = 0x03, //!< Write digital outputs
        READ_DIO                = 0x04, //!< Read digital inputs
        WRITE_SERVO             = 0x05, //!< Write servo outputs
        READ_ANALOG             = 0x06, //!< Read analog inputs
        SET_RS485_BAUD          = 0x07, //!< Configure RS485 baud rate
        DO_RS485_COMM           = 0x08, //!< Send data to devices on the RS485 bus
        WRITE_I2C               = 0x09, //!< Write to a device on the i2c bus
        READ_I2C                = 0x0A, //!< Read from a device on the i2c bus
        SET_MOTOR_PARAM         = 0x0B, //!< Configure motor drive-train parameters
        WRITE_MOTOR_SPEED       = 0x0C, //!< Set motor speed and direction
        READ_MOTOR_SPEED        = 0x0D, //!< Get motor speed
        READ_MOTOR_CURRENT      = 0x0E, //!< Get motor currents
        SET_MOTOR_PID_GAINS     = 0x0F, //!< Configure motor controller PID gains
        SET_MOTOR_DRIVEMODE     = 0x10, //!< Set direct drive or PID mode for motor controller
        WRITE_MOTOR_POWER       = 0x11, //!< Set motor power
        READ_MOTOR_ENCODERS     = 0x12, //!< Read motor drive encoders
        READ_BOARD_VERSION      = 0xFF  //!< Get version information
    };

protected:
    IoBoardCommand() {}
    virtual ~IoBoardCommand() {}

    /// Create command message given message code and payload
    /// message format: [header(3)][payload length(1)][cmd(1)][payload(n)][checksum(1)]
    /// \param cmd Command code (IoBoardCommand::Command)
    /// \param nPayloadBytes Number of bytes in the payload (0-255).
    /// \param pPayloadData Pointer to array containing payload data. If set to null,
    ///                     payload data is set to 0.
    void createCommand(Command cmd, int nPayloadBytes, char* pPayloadData);

    /// Derived classes must always call this method whenever it modifies the command
    /// message buffer in any way.
    void setCommandModified();

private:
    char computeChecksum();

}; // IoBoardCommand


/// \class ReadBoardVersionCommand
/// \ingroup comms
/// \brief Create command to read board version info.
class UGV1_DLL_API ReadBoardVersionCommand : public IoBoardCommand
{
public:
    ReadBoardVersionCommand() : IoBoardCommand()
    {
        createCommand(READ_BOARD_VERSION,0,NULL);
    }
};

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_H
