//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand_dio.h
// Brief    : UGV1 driver board command messages - digital IO
//==============================================================================

#ifndef UGV1_IOBOARDCOMMAND_DIO_H
#define UGV1_IOBOARDCOMMAND_DIO_H

#include "IoBoardCommand.h"

namespace Ugv1
{

/// \class SetDioIoModeCommand
/// \ingroup comms
/// \brief Configure IO pins as digital inputs and outputs
class UGV1_DLL_API SetDioIoModeCommand : public IoBoardCommand
{
public:
    /// Default constructor. Sets all digital pins as inputs.
    SetDioIoModeCommand();

    /// Constructs command where specified pins are set as digital output
    /// \param bitmask Bitmask to specify digital pins that are to be configured
    ///                 as outputs. There are only 11 IO lines on the board.
    ///                 Therefore only the 11 least significant bits are considered.
    ///                 Bit 0 is IO0.
    SetDioIoModeCommand(int bitmask);

    /// Set specified channel as digital input
    /// \param channel  The channel number in range (0 - 10)
    /// \param option   Set this to true to configure IO pin as input
    /// \see setModeOutput, isModeInput
    void setModeInput(int channel, bool option);

    /// \return true if the specified channel (0 - 10) is set to be digital input
    bool isModeInput(int channel);

    /// Set specified channel as digital output
    /// \param channel  The channel number in range (0 - 10)
    /// \param option   Set this to true to configure IO pin as output
    /// \see setModeInput, isModeOutput
    void setModeOutput(int channel, bool option);

    /// \return true if the specified channel (0 - 10) is set to be digital output
    bool isModeOutput(int channel);

}; // SetDioIoModeCommand

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_DIO_H
