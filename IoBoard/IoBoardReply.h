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
