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
/// \ingroup vehicle
/// \brief Base class for UGV1 I/O board messages
class UGV1_DLL_API IoBoardMessage : public std::vector<char>
{
public:
    static const char MESSAGE_HEADER[3];

public:
    IoBoardMessage();
    virtual ~IoBoardMessage();    
};

} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_H
