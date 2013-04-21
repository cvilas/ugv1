//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoardMessage.cpp
//==============================================================================

#include "IoBoardMessage.h"

namespace Ugv1
{

const unsigned char IoBoardMessage::MESSAGE_HEADER[3] = {0x55, 0xAA, 0x10};

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
IoBoardMessage::MessageID IoBoardMessage::getIdFromMessage() const
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
    std::vector<unsigned char>::iterator it = begin();
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
unsigned char IoBoardCommand::computeChecksum()
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
    return( (unsigned char)(csum&0xFF));
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
        std::cerr << "[IoBoardResponse::isValid] invalid size " << size() << " expected "  << getExpectedLength() << std::endl;
        return false;
    }

    iterator it = begin();
    if( *it != MESSAGE_HEADER[0] )
    {
        std::cerr << "[IoBoardResponse::isValid] invalid header 0" << std::endl;
        return false;
    }
    ++it;
    if( *it != MESSAGE_HEADER[1] )
    {
        std::cerr << "[IoBoardResponse::isValid] invalid header 1" << std::endl;
        return false;
    }
    ++it;
    if( *it != MESSAGE_HEADER[2] )
    {
        std::cerr << "[IoBoardResponse::isValid] invalid header 2" << std::endl;
        return false;
    }

    if( !verifyChecksum() )
    {
        std::cerr << "[IoBoardResponse::isValid] invalid checksum" << std::endl;
        return false;
    }

    if( !verifyId() )
    {
        std::cerr << "[IoBoardResponse::isValid] invalid ID" << std::endl;
        return false;
    }

    return true;
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
