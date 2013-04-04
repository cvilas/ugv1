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
/// \ingroup comms
/// \brief Base class for UGV1 I/O board messages
/// See Sensor/Motor Drive Board - Version 2.2 (SKU:DFR0057) Wiki for command format:
/// in docs/Sensor Driver Board V2 - wiki.pdf, or the webpage
/// http://www.dfrobot.com/wiki/index.php?title=Sensor/Motor_Drive_Board_-_Version_2.2_(SKU:DFR0057)
class UGV1_DLL_API IoBoardMessage : public std::vector<char>
{
public:

    /// header bytes common to all messages
    static const char MESSAGE_HEADER[3];

    /// 0 based index to first element in payload part of the message
    static const unsigned int MESSAGE_PAYLOAD_INDEX = 5;

public:
    IoBoardMessage();
    virtual ~IoBoardMessage();    
};

} // Ugv1

#endif // UGV1_IOBOARDMESSAGE_H
