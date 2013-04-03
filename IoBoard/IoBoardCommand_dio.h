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
/// \brief Configure digital IO pins as inputs and outputs
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

/// \class WriteDioOutCommand
/// \ingroup comms
/// \brief Command to write digital outputs
class UGV1_DLL_API WriteDioOutCommand : public IoBoardCommand
{
public:
    /// Default constructor. Creates command with all output pins set to low.
    WriteDioOutCommand();

    /// Constructs command where pins are set to specified outputs
    /// \param bitmask  Set bits in this mask to 0 or 1 to set corresponding pin low or high, respectively.
    ///                 There are only 11 IO lines on the board. Therefore only the 11 least significant bits
    ///                 are considered. Bit 0 is IO0. If one of the DIO is configured as Digital Input Mode,
    ///                 the command to set its output will be invalid.
    WriteDioOutCommand(int bitmask);

    /// Set a specific digital output high or low. If the specified channel is configured as a digital
    /// input, the command is invalid.
    /// \param channel  Pin number in range 0 - 10
    /// \param high     Value. Set to true for high.
    void setChannel(int channel, bool high);

    /// \return The output set for the specified channel
    bool getChannel(int channel);

}; // WriteDioOutCommand

/// \class ReadDioInCommand
/// \ingroup comms
/// \brief Command a read of digital lines configured as inputs
class UGV1_DLL_API ReadDioInCommand : public IoBoardCommand
{
public:
    ReadDioInCommand();
}; // ReadDioInCommand

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_DIO_H
