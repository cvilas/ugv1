//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : MessengerSubscriber.cpp
// Brief    : Example program for UGV1 AgentMessenger
//==============================================================================

#include "AgentExamples.h"
#include "AgentMessenger.h"
#include "example_t.hpp"
#include <utils/consoleio.h>
#include <iostream>
#include <timing/StopWatch.h>

//==============================================================================
// Handles messages of type example_t
class MessageHandler
//==============================================================================
{
public:
    MessageHandler() {}
    ~MessageHandler() {}

    // message handler
    void onExampleMessage(const lcm::ReceiveBuffer* rBuf,
                          const std::string& channel,
                          const example_t* pMsg)
    {
        std::cout << "Message received on channel "
                  << channel << " at " << rBuf->recv_utime << " us" << std::endl;
        std::cout << "\tTimestamp = " << (long long)pMsg->timestamp << std::endl;
        std::cout << "\tPosition  = " << pMsg->position[0]
                  << ", " << pMsg->position[1]
                  << ", " << pMsg->position[2] << std::endl;
        std::cout << "\tOrientation = " << pMsg->orientation[0]
                  << ", " << pMsg->orientation[1]
                  << ", " << pMsg->orientation[2]
                  << ", " << pMsg->orientation[3] << std::endl;
        std::cout << "\tName = " << pMsg->name << std::endl;
        std::cout << "\tEnabled = " << ((pMsg->enabled)?(1):(0)) << std::endl;
    }

};

//==============================================================================
int main()
//==============================================================================
{
    Ugv1::AgentMessenger subscriber;
    MessageHandler handler;

    if( !subscriber.isGood() )
    {
        std::cout << "Error setting up messaging for subscriber" << std::endl;
        return false;
    }

    // subscribe to a channel
    subscriber.subscribe(EXAMPLE_CHANNEL, &MessageHandler::onExampleMessage, &handler, 20);

    std::cout << "Waiting for messages. Press any key to exit." << std::endl;
    while( !Grape::kbhit() )
    {
        Grape::StopWatch::nanoSleep(1000000000);
    }

    std::cout << "Subscriber exiting..." << std::endl;

    return true;
}

