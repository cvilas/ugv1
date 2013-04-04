//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardReply.h
// Brief    : UGV1 driver board reply messages
//==============================================================================

#ifndef UGV1_IOBOARDREPLY_H
#define UGV1_IOBOARDREPLY_H

#include "IoBoardCommand.h"

namespace Ugv1
{

/*
READ_DIO                = 0x04, //!< Read digital inputs
READ_ANALOG             = 0x06, //!< Read analog inputs
DO_RS485_COMM           = 0x08, //!< Send data to devices on the RS485 bus
READ_I2C                = 0x0A, //!< Read from a device on the i2c bus
READ_MOTOR_SPEED        = 0x0D, //!< Get motor speed
READ_MOTOR_CURRENT      = 0x0E, //!< Get motor currents
READ_MOTOR_ENCODERS     = 0x12, //!< Read motor drive encoders
READ_BOARD_VERSION      = 0xFF  //!< Get version information
*/

/// \ingroup comms
class UGV1_DLL_API IoBoardReply : public IoBoardMessage
{
public:
    IoBoardReply();
    virtual ~IoBoardReply();
    bool verifyChecksum();
}; // IoBoardReply

} // namespace Ugv1




#endif // UGV1_IOBOARDREPLY_H
