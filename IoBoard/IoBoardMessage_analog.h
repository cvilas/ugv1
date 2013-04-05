//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage_analog.h
// Brief    : UGV1 driver board messages - analog commands
//==============================================================================

#ifndef UGV1_IOBOARDMESSAGE_ANALOG_H
#define UGV1_IOBOARDMESSAGE_ANALOG_H

#include "IoBoardMessage.h"

namespace Ugv1
{

/// \class ReadAnalogInCommand
/// \ingroup comms
/// \brief Command a read of analog inputs
class UGV1_DLL_API ReadAnalogInCommand : public IoBoardCommand
{
public:
    ReadAnalogInCommand();
}; // ReadAnalogInCommand


} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_ANALOG_H
