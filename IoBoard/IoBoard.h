//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoard.h
// Brief    : IO board interface
//==============================================================================

#ifndef UGV1_IOBOARD_H
#define UGV1_IOBOARD_H

#include "IoBoardReply.h"
#include "IPort.h"

namespace Ugv1
{

class UGV1_DLL_API IoBoard
{

public:
    IoBoard(Grape::IPort& transport);
    ~IoBoard();

    bool send(IoBoardCommand& cmd, IoBoardReply& reply);
    bool send(IoBoardCommand& cmd);
private:
    IoBoard(const IoBoard&); // disable copy
    IoBoard& operator=(const IoBoard&); // disable assignment
}; //IoBoard

} // namespace Ugv1

#endif // UGV1_IOBOARD_H
