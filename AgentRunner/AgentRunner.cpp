//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentRunner.cpp
// Brief    : Loads and runs an Agent on the command line
//==============================================================================

#include <iostream>
#include "IAgent.h"
#include <AgentBus.h>
#include <utils/consoleio.h>

#include <QCoreApplication>
#include <QStringList>
#include <QLibrary>

//==============================================================================
int main(int argc, char** argv)
//==============================================================================
{
    QCoreApplication app(argc, argv);
    QStringList args = app.arguments();

    if( args.size() != 5 )
    {
        std::cerr << "Usage: " << std::endl
                  << "\t" << argv[0] << " -config <configFile> -agent lib<agent>.so"
                  << std::endl;
        return -1;
    }

    std::string configFile = "../config/ugv1.cfg";
    std::string agentFile = "";

    // get config file
    int i = 1 + args.indexOf("-config");
    if( (i != 0) && (i < args.size()) )
    {
        configFile = args.at(i).toStdString();
    }

    // get lib file
    i = 1 + args.indexOf("-agent");
    if( (i != 0) && (i < args.size()) )
    {
        agentFile = args.at(i).toStdString();
    }

    if( agentFile.empty() )
    {
        std::cerr << "[" << argv[0] << "] Agent file not specified" << std::endl;
        return -1;
    }

    // initialise
    Ugv1::AgentBus bus(configFile);

    // load library
    QLibrary agentLib(agentFile.c_str());
    if( !agentLib.load() )
    {
        std::cerr << "[" << argv[0] << "] Error loading " << agentFile << std::endl;
        std::cerr << agentLib.errorString().toStdString() << std::endl;
        return -1;
    }

    // create the object
    typedef Ugv1::IAgent* (*createInstance_t)(Ugv1::AgentBus&);
    createInstance_t createInstance = (createInstance_t) agentLib.resolve("createAgent");
    if (!createInstance )
    {
        std::cerr << "[" << argv[0] << "] No createAgent found in "
                  << agentFile << std::endl;
        return -1;
    }

    Ugv1::IAgent* pAgent = createInstance(bus);
    if( !pAgent )
    {
        std::cerr << "[" << argv[0] << "] Error instantiating agent" << std::endl;
        return -1;
    }

    // configure
    try
    {
        pAgent->configure();
        pAgent->start();

        std::cout << "[" << argv[0] << "] Press Q to exit" << std::endl;

        while( 1 )
        {
            if( Grape::kbhit() )
            {
                char k = Grape::getch();
                if( (k == 'q') || (k == 'Q') )
                {
                    break;
                }
            }
            app.processEvents();
        }

        // cleanup
        pAgent->stop();
    }
    catch(Ugv1::AgentException &ex)
    {
        std::cerr << "AgentRunner caught exception:" << std::endl << ex.what() << std::endl;
    }

    typedef void (*deleteInstance_t)(Ugv1::IAgent*);
    deleteInstance_t deleteInstance = (deleteInstance_t) agentLib.resolve("deleteAgent");
    if( deleteInstance )
    {
        deleteInstance(pAgent);
    }

    return 0;
}
