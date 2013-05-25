//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : MessengerPublisher.cpp
// Brief    : Example program for UGV1 AgentMessenger
//==============================================================================

#include "AgentExamples.h"
#include "AgentMessenger.h"
#include "example_t.hpp"
#include <iostream>

//==============================================================================
int main()
//==============================================================================
{
    Ugv1::AgentMessenger publisher;

    if( !publisher.isGood() )
    {
        std::cout << "Error setting up messaging for publisher" << std::endl;
        return -1;
    }

	// the message
    example_t my_data;
    my_data.timestamp = 0;

    my_data.position[0] = 1;
    my_data.position[1] = 2;
    my_data.position[2] = 3;

    my_data.orientation[0] = 1;
    my_data.orientation[1] = 0;
    my_data.orientation[2] = 0;
    my_data.orientation[3] = 0;

    my_data.num_ranges = 15;
    my_data.ranges.resize(my_data.num_ranges);
    for(int i = 0; i < my_data.num_ranges; i++)
	{
        my_data.ranges[i] = i;
	}
	
    my_data.name = "example string";
    my_data.enabled = true;

	
    publisher.publish(EXAMPLE_CHANNEL, &my_data);

    return 0;
}

