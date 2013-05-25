//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : JoystickSubscriber2.cpp
// Brief    : Example program for UGV1 JoystickAgent
//==============================================================================

#include "AgentBus.h"
#include "Ugv1Messages/JoyMessage.hpp"
#include <utils/consoleio.h>
#include <QCoreApplication>
#include <QStringList>
#include <iostream>

//==============================================================================
// Subscribes to messages of type JoyMessage
class Handler
//==============================================================================
{
public:
    Handler() {}
    ~Handler() {}

    // message handler
    void onJoyMessage(const lcm::ReceiveBuffer* rBuf,
                          const std::string& channel,
                          const Ugv1Messages::JoyMessage* pMsg)
    {
        std::cout << channel << " [" << rBuf->recv_utime << "] ["
                  << pMsg->uTime << "] ("
                  << pMsg->rawSurgeRate << ", "
                  << pMsg->rawYawRate << ") ("
                  << (int)pMsg->deadMansHandle << ")" << std::endl;
    }

};


//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{

    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();

    if( args.size() != 3 )
    {
        std::cerr << "Usage: " << std::endl
                  << "\t" << argv[0] << " -config <configFile>"
                  << std::endl;
        return -1;
    }

    std::string configFile = "../config/ugv1_linux.cfg";

    // get config file
    int i = 1 + args.indexOf("-config");
    if( (i != 0) && (i < args.size()) )
    {
        configFile = args.at(i).toStdString();
    }

    Ugv1::AgentBus bus(configFile);

    // Just use the messenger in the node manager itself.
    // Note: We could have used a different messenger. See
    // JoystickNodeSubscriber.cpp
    Ugv1::AgentMessenger* pMessenger = bus.getMessenger();
    if( !pMessenger )
    {
        std::cerr << "[" << argv[0] << "No Messenge to use" << std::endl;
        return false;
    }

    // Following error is highly unlikely (or CPuceNodeManager::initialise()
    // would have failed). But test anyway.
    if( !pMessenger->isGood() )
    {
        std::cerr << "[" << argv[0] << "Messenger no good" << std::endl;
        return false;
    }

    // subscribe to the channel transmitting JoyMessage
    std::string channel = "JOYSTICK"; //this comes from the configuration file

    Handler handler;
    pMessenger->subscribe(channel, &Handler::onJoyMessage, &handler, 20);

    // start processing.

    // Again, following error is unlikely.
    if( !pMessenger->isRunning() )
    {
        std::cerr << "[" << argv[0] << "] message handler not running" << std::endl;
        return -1;
    }

    std::cout << "[" << argv[0] << "] Started. Press any key to exit." << std::endl;

    while( !Grape::kbhit() )
    {
        app.processEvents();
    }

    return 0;
}

