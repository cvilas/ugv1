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

} // Ugv1

#endif // UGV1_IOBOARDCOMMAND_H
