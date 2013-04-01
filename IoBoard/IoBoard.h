//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoard.h
// Brief    : IO board interface
//==============================================================================

#ifndef UGV1_IOBOARD_H
#define UGV1_IOBOARD_H

#include "ugv1_common.h"
#include "io/IPort.h"

namespace Ugv1
{

class UGV1_DLL_API IoBoard
{

public:
    IoBoard(Grape::IPort& transport);
    ~IoBoard();

    bool send(IoBoardCommand& cmd, IoBoardReply& reply);
    bool send(IoBoardCommand& cmd);
}; //IoBoard

} // namespace Ugv1

#endif // UGV1_IOBOARD_H
