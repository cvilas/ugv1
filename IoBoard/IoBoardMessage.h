//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage.h
// Brief    : base class for UGV1 driver board messages
//==============================================================================

#ifndef UGV1_IOBOARDMESSAGE_H
#define UGV1_IOBOARDMESSAGE_H

#include "ugv1_common.h"
#include <vector>

namespace Ugv1
{

/// \class IoBoardMessage
/// \ingroup comms
/// \brief Base class for UGV1 I/O board messages
/// See Sensor/Motor Drive Board - Version 2.2 (SKU:DFR0057) Wiki for command format:
/// in docs/Sensor Driver Board V2 - wiki.pdf, or the webpage
/// http://www.dfrobot.com/wiki/index.php?title=Sensor/Motor_Drive_Board_-_Version_2.2_(SKU:DFR0057)
class UGV1_DLL_API IoBoardMessage : public std::vector<char>
{
public:

    /// header bytes common to all messages
    static const char MESSAGE_HEADER[3];

    /// 0 based index to element containing message identifier
    /// \see IoBoardCommand::MessageID
    static const unsigned int MESSAGE_ID_INDEX = 4;

    /// 0 based index to first element in payload part of the message
    static const unsigned int MESSAGE_PAYLOAD_INDEX = 5;

public:

    /// Message types supported by the sensor/motor IO board
    enum MessageID
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
        RESET_MOTOR_ENCODERS    = 0x13, //!< Reset motor drive encoder counters
        READ_BOARD_VERSION      = 0xFF  //!< Get version information
    };

public:
    IoBoardMessage();
    virtual ~IoBoardMessage();    
};

/// \class IoBoardCommand
/// \ingroup comms
/// \brief IO board command message builder. Base class for all IoBoard commands.
/// \see IoBoardReply, IoBoardMessage
class UGV1_DLL_API IoBoardCommand : public IoBoardMessage
{

protected:
    IoBoardCommand() {}
    virtual ~IoBoardCommand() {}

    /// Create command message given message code and payload
    /// message format: [header(3)][payload length(1)][cmd(1)][payload(n)][checksum(1)]
    /// \param cmd Command code (IoBoardCommand::MessageID)
    /// \param nPayloadBytes Number of bytes in the payload (0-255).
    /// \param pPayloadData Pointer to array containing payload data. If set to null,
    ///                     payload data is set to 0.
    void initialise(MessageID cmd, int nPayloadBytes, char* pPayloadData);

    /// Derived classes must always call this method whenever it modifies the command
    /// message buffer in any way.
    void setCommandModified();

private:
    char computeChecksum();

}; // IoBoardCommand

/// \class IoBoardResponse
/// \ingroup comms
/// \brief Response from the IoBoard for IoBoardCommand messages
class UGV1_DLL_API IoBoardResponse : public IoBoardMessage
{
public:
    IoBoardResponse();
    virtual ~IoBoardResponse();
    bool isValid();
protected:
    virtual bool verifyId() = 0;
    virtual size_t getExpectedLength() = 0;
    bool verifyChecksum();
}; // IoBoardResponse

/// \class ReadBoardVersionCommand
/// \ingroup comms
/// \brief Create command to read board version info.
class UGV1_DLL_API ReadBoardVersionCommand : public IoBoardCommand
{
public:
    ReadBoardVersionCommand() : IoBoardCommand()
    {
        initialise(READ_BOARD_VERSION,0,NULL);
    }
};

class ReadAnalogInResponse : public IoBoardResponse
{};

class ReadMotorSpeedResponse : public IoBoardResponse
{};

class ReadMotorCurrentResponse : public IoBoardResponse
{};

class ReadMotorEncodersResponse : public IoBoardResponse
{};

class ReadBoardVersionResponse : public IoBoardResponse
{};


} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_H
