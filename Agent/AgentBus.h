//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentBus.h
// Brief    : Communication bus for multiple agents
//==============================================================================

#ifndef UGV1_AGENTBUS_H
#define	UGV1_AGENTBUS_H

#include "AgentException.h"
#include "AgentMessenger.h"
#include <map>
#include <QtXml/QDomDocument>

namespace Ugv1
{

/// \class AgentBus
/// \ingroup agent
/// \brief Communication bus for multiple agents
///
/// AgentBus provides a common bus over which multiple agents can communicate
/// with each other. The messaging backend is LCM (https://code.google.com/p/lcm/)
/// Connected agents can configure themselves by reading the configuration object
/// from this class.
class UGV1AGENT_DLL_API AgentBus
{
public:

    /// Constructor. Reads configuration file and
    /// initialises the message bus
    AgentBus(const std::string& cfgFileName) throw(AgentException);

    ~AgentBus();

    /// \return URL of the network bus over which connected agents
    /// are communicating.
    std::string getUrl() { return _url; }

    /// \return The configuration document. This is used by connected agents
    ///         to configure themselves
    QDomDocument& getConfig() { return _config; }

    /// \return The message type for the specified channel
    std::string getMessageType(const std::string& channel);

private:
    void createMessenger() throw(AgentException); // Create the messaging backend
    AgentBus(const AgentBus &);               //!< disable copy
    AgentBus &operator=(const AgentBus &);    //!< disable assignment

private:
    QDomDocument    _config;
    AgentMessenger* _pMessenger;
    std::string     _url;
    std::map<std::string/*channel*/, std::string/*message type*/> _channels;
}; // AgentBus

} // Ugv1

#endif //UGV1_AGENTBUS_H
