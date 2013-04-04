//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardCommand.cpp
//==============================================================================

#include "IoBoardCommand.h"
#include <cstddef>

namespace Ugv1
{

//=============================================================================
void IoBoardCommand::initialise(MessageID cmd, int nPayloadBytes, char* payload)
//=============================================================================
{
    // message format: [header(3)][payload length(1)][cmd(1)][payload(n)][checksum(1)]

    resize(nPayloadBytes + 6);
    std::vector<char>::iterator it = begin();
    *it = IoBoardMessage::MESSAGE_HEADER[0]; ++it;
    *it = IoBoardMessage::MESSAGE_HEADER[1]; ++it;
    *it = IoBoardMessage::MESSAGE_HEADER[2]; ++it;
    *it = nPayloadBytes; ++it;
    *it = cmd; ++it;
    for( int i = 0; i < nPayloadBytes; ++i)
    {
        (payload != NULL) ? (*it = payload[i]) : (*it = 0);
        ++it;
    }
    *it = computeChecksum();
}

//-----------------------------------------------------------------------------
void IoBoardCommand::setCommandModified()
//-----------------------------------------------------------------------------
{
    *(end()-1) = computeChecksum();
}

//-----------------------------------------------------------------------------
char IoBoardCommand::computeChecksum()
//-----------------------------------------------------------------------------
{
    unsigned int csum = 0;
    const_iterator it = begin();
    const_iterator itEnd = end() - 1; // checksum is the last byte
    while( it != itEnd )
    {
        csum += (unsigned int)(*it);
        ++it;
    }
    return( (char)(csum&0xFF));
}

} // Ugv1
