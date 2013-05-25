//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : IAgent.h
// Brief    : Abstract interaface for a computation/communication node
//==============================================================================

#ifndef UGV1_IAGENT_H
#define	UGV1_IAGENT_H

#include "AgentException.h"

namespace Ugv1
{

class AgentBus;
class IAgent;

extern "C"
{
/// Helper function for the dynamic loader - deletes agent when called. The following
/// function must also be provided in the implementation of agents
/// \code
/// extern "C" { IAgent* createAgent(AgentBus& man); }
/// \endcode
UGV1AGENTLIB_DLL_API void deleteAgent(IAgent* ptr);
}

/// \class IAgent
/// \ingroup agent
/// \brief Abstract interface for communication/computation nodes
///
/// At the fundamental level, UGV1 consists of distributed software agents that
/// communicate over a system bus using a publish/subscribe mechanism. Agents are
/// implemented using a plugin type architecture. A specific implementation of
/// an agent is compiled into a shared library, and is loaded at run-time using
/// the AgentRunner program. Agents have the ability to configure themselves by
/// reading a configuration file, and can be started and stopped at will without
/// effecting the rest of the system.
class UGV1AGENTLIB_DLL_API IAgent
{
public:
    virtual ~IAgent() {}

    /// Read the configuration object from the bus (getBus().getConfig())
    /// and initialise the node. The agent shouldn't start processing
    /// until start() is called.
    virtual void configure() throw(AgentException) = 0;

    /// Initiate node data processing and return immediately.
    /// The actual data processing itself may happen else where (eg. a separate
    /// thread).
    virtual void start() throw(AgentException) = 0;

    /// Stop processing.
    virtual void stop() throw() = 0;

protected:
    IAgent(AgentBus& man) throw(AgentException);

    /// \return The bus over which this agent is commicating with others.
    AgentBus& getBus() { return _agentBus; }

private:
    IAgent(const IAgent &);               //!< disable copy
    IAgent &operator=(const IAgent &);    //!< disable assignment
private:
    AgentBus& _agentBus;
}; // IAgent

} // Ugv1

#endif // UGV1_IAGENT_H
