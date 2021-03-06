//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardMessage_analog.h
// Brief    : UGV1 driver board messages - analog commands
//==============================================================================

#ifndef UGV1_IOBOARDMESSAGE_ANALOG_H
#define UGV1_IOBOARDMESSAGE_ANALOG_H

#include "IoBoardMessage.h"

namespace Ugv1
{

/// \class ReadAnalogInCommand
/// \ingroup cont_comms
/// \brief Command a read of analog inputs
class UGV1CONTROLLER_DLL_API ReadAnalogInCommand : public IoBoardCommand
{
public:
    ReadAnalogInCommand();
}; // ReadAnalogInCommand

/// \class ReadAnalogInResponse
/// \ingroup cont_comms
/// \brief Response message for ReadAnalogInCommand
class UGV1CONTROLLER_DLL_API ReadAnalogInResponse : public IoBoardResponse
{
public:
    ReadAnalogInResponse() : IoBoardResponse() {}
    size_t getExpectedLength() { return 23; }
    MessageID getExpectedId() { return READ_ANALOG; }

    /// Read the raw ADC input for a specified channel.
    /// \note The ADC value is 12 bits long. 0x0000 maps to 0V and 0x0FFF maps to 3.3V
    /// \param channel  ADC channel in range 0 - 7.
    /// \return ADC value in range 0x0000 to 0x0FFF
    unsigned short getCount(unsigned int channel);

    /// Convenience function, returns value directly in volts (0.0 - 3.3V)
    double getVolts(unsigned int channel);
};

} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_ANALOG_H
