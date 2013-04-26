//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardMessage.cpp
//==============================================================================

#include "IoBoardMessage.h"
#include <sstream>

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
void IoBoardResponse::validate() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    // check length
    // check header
    // check ID
    // verify checksum

    if( size() != getExpectedLength() )
    {
		std::ostringstream str;
        str << __FUNCTION__ << ": For message ID " << getExpectedId() << ", expected message length " 
			<< getExpectedLength() << " got " << size();
        throw InvalidMessageLengthException(0, str.str());
    }

    iterator it = begin();
    if( *it != MESSAGE_HEADER[0] )
    {
		std::ostringstream str;
        str << __FUNCTION__ << ": For message ID " << getExpectedId() << ", expected header[0] " 
			<< ((int)(*it)&0xFF) << " got " << ((int)(MESSAGE_HEADER[0])&0xFF);
        throw InvalidMessageHeaderException(0, str.str());
    }
    ++it;
    if( *it != MESSAGE_HEADER[1] )
    {
		std::ostringstream str;
        str << __FUNCTION__ << ": For message ID " << getExpectedId() << ", expected header[1] " 
			<< ((int)(*it)&0xFF) << " got " << ((int)(MESSAGE_HEADER[1])&0xFF);
        throw InvalidMessageHeaderException(0, str.str());
    }
    ++it;
    if( *it != MESSAGE_HEADER[2] )
    {
		std::ostringstream str;
        str << __FUNCTION__ << ": For message ID " << getExpectedId() << ", expected header[2] " 
			<< ((int)(*it)&0xFF) << " got " << ((int)(MESSAGE_HEADER[2])&0xFF);
        throw InvalidMessageHeaderException(0, str.str());
    }

    if( !verifyId() )
    {
        std::ostringstream str;
		str << __FUNCTION__ << ": Expected message ID " << getExpectedId() << ", got " << getIdFromMessage();
        throw InvalidMessageIdException(0,str.str());
    }

    if( !verifyChecksum() )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": For message ID " << getExpectedId() << ", invalid checksum received";
        throw InvalidMessageChecksumException(0,str.str());
    }
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
