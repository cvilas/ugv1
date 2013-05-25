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
/// from this class, and get a pointer to the messaging backend to publish/subscribe
/// messages on the bus.
class UGV1AGENTLIB_DLL_API AgentBus
{
public:

    /// Constructor. Reads configuration file and initialises the message bus.
    /// The configuration file should contain a <MessageBus> section
    /// that looks something like this:
    ///
    /// \code
    /// <?xml version="1.0" encoding="UTF-8" ?>
    ///<Ugv1>
    ///<!-- Message bus configuration and channels available -->
    /// <MessageBus Url="udpm://239.255.76.67:7667?ttl=0">
    ///     <Channel Name="JOYSTICK" Type="JoyMessage"></Channel>
    ///     <Channel Name="COMMAND" Type="CommandMessage"></Channel>
    ///     <Channel Name="CAMERA" Type="ImageMessage"></Channel>
    /// </MessageBus>
    ///</Ugv1>
    /// \endcode
    AgentBus(const std::string& cfgFileName) throw(AgentException);

    ~AgentBus();

    /// \return URL of the network bus over which connected agents
    ///         are communicating.
    std::string getUrl() { return _url; }

    /// \return The configuration document. This is used by connected agents
    ///         to configure themselves
    QDomDocument& getConfig() { return _config; }

    /// \return The message type for the specified channel
    std::string getMessageType(const std::string& channel);

    /// \return Pointer to backend messenger. This could be NULL if the messenger
    ///         isn't configured properly
    AgentMessenger* getMessenger() { return _pMessenger; }

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
