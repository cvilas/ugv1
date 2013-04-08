//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage.cpp
//==============================================================================

#include "IoBoardMessage.h"

namespace Ugv1
{

const char IoBoardMessage::MESSAGE_HEADER[3] = {0x55, 0xAA, 0x10};

//==============================================================================
IoBoardMessage::IoBoardMessage()
//==============================================================================
{
}

//-----------------------------------------------------------------------------
IoBoardMessage::~IoBoardMessage()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
IoBoardMessage::MessageID IoBoardMessage::getId() const
//-----------------------------------------------------------------------------
{
    if( size() < (MESSAGE_ID_INDEX+1))
        return ID_UNKNOWN;

    return (IoBoardMessage::MessageID)(*(begin() + MESSAGE_ID_INDEX)&0xFF);
}

//=============================================================================
void IoBoardCommand::initialise(MessageID cmd, int nPayloadBytes, char* payload)
//=============================================================================
{
    // message format: [header(3)][payload length(1)][cmd(1)][payload(n)][checksum(1)]

    resize(nPayloadBytes + COMMAND_MIN_LENGTH);
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

//=============================================================================
IoBoardResponse::IoBoardResponse()
//=============================================================================
{
}

//-----------------------------------------------------------------------------
IoBoardResponse::~IoBoardResponse()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
bool IoBoardResponse::isValid()
//-----------------------------------------------------------------------------
{
    // check length
    // check header
    // check ID
    // verify checksum

    if( size() != getExpectedLength() )
    {
        return false;
    }

    iterator it = begin();
    if( *it != MESSAGE_HEADER[0] )
    {
        return false;
    }
    ++it;
    if( *it != MESSAGE_HEADER[1] )
    {
        return false;
    }
    ++it;
    if( *it != MESSAGE_HEADER[2] )
    {
        return false;
    }

    return (verifyChecksum() && verifyId());
}

//-----------------------------------------------------------------------------
bool IoBoardResponse::verifyChecksum()
//-----------------------------------------------------------------------------
{
    const_iterator itCSum = end() - 2; // checksum is second last character

    unsigned int csum = 0;
    const_iterator it = begin();
    while( it != itCSum )
    {
        csum += (unsigned int)(*it);
        ++it;
    }
    return ( ((*itCSum)&0xFF) == (csum&0xFF) );
}

} // Ugv1
