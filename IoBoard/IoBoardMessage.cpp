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
bool IoBoardMessage::verifyChecksum()
//-----------------------------------------------------------------------------
{
    const_iterator itCSum = end() - 2; // checksum is second last character

    int csum = 0;
    const_iterator it = begin();
    while( it != itCSum )
    {
        csum += (int)(*it);
        ++it;
    }

    return ( (*itCSum) == (csum&0xFF) );

}

//-----------------------------------------------------------------------------
void IoBoardMessage::addChecksum()
//-----------------------------------------------------------------------------
{
    int csum = 0;
    const_iterator it = begin();
    while( it != end() )
    {
        csum += (int)(*it);
        ++it;
    }
    push_back( (char)(csum&0xFF));
}

} // Ugv1
