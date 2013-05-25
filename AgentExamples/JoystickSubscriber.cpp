//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : JoystickSubscriber.cpp
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

    // We are on this network
    std::string url = bus.getUrl();

    // create a new messenger on the same network
    // Note: We could have used the messenger from AgentManager. See
    // JoystickSubscriber2.cpp
    Ugv1::AgentMessenger messenger( url );
    if( !messenger.isRunning() )
    {
        std::cerr << "[" << argv[0] << "] messenger thread failed" << std::endl;
        return -1;
    }

    if( !messenger.isGood() )
    {
        std::cerr << "[" << argv[0] << "Messenger no good" << std::endl;
        return false;
    }

    // subscribe to the channel transmitting JoyMessage
    std::string channel = "JOYSTICK"; // this comes from the configuration file

    Handler handler;
    messenger.subscribe(channel, &Handler::onJoyMessage, &handler, 20);

    // start processing
    std::cout << "[" << argv[0] << "] Started. Press any key to exit." << std::endl;
    while( !Grape::kbhit() )
    {
        app.processEvents();
    }

    return 0;
}

