//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentMessenger.h
// Brief    : Communication backend mechanism
//==============================================================================

#ifndef UGV1_AGENTMESSENGER_H
#define	UGV1_AGENTMESSENGER_H

#include "AgentException.h"
#include "lcm/lcm-cpp.hpp"
#include <QThread>
#include <QMutex>

namespace Ugv1
{

/// \class AgentMessenger
/// \ingroup agent
/// \brief Communication backend
///
/// The messenger backend is a wrapper over LCM (https://code.google.com/p/lcm/). Distributed
/// agents communicate with each other through a publish/subscribe mechanism that is
/// provided by this class. An instance of this class can be utilised to communicate
/// messages over one of more 'channels', where each channel carries messages
/// of a particular 'type'.
///
/// Read the LCM C++ API documentation for details:
/// http://lcm.googlecode.com/svn/www/reference/lcm/index.html.
///
/// <b>Example programs:</b>
/// Publisher:
/// \include MessagePublisher.cpp
/// Subscriber:
/// \include MessageSubscriber.cpp
class UGV1AGENT_DLL_API AgentMessenger : public QThread
{
public:

    /// Creates the messenger.
    /// \param url (input) A url string describing the service provided of the
    ///        form
    /// \code
    /// "provider://network?option1=value1&option2=value2&...&optionN=valueN"
    /// \endcode
    /// where "provider" can be "udpm://" (UDP Multicast provider) or "file://"
    /// (Log file based provider).
    ///
    /// UDP messenger supports the following options:
    /// - recv_buf_size = N (size of the kernel UDP receive buffer to request)
    /// - ttl = N (time to live for transmitted packets. Default = 0. Set to non
    ///         zero to let published packets enter the network from the host)
    /// - Eg: "udpm://239.255.76.67:7667?ttl=1"
    ///
    /// The file based message source supports the following options:
    /// - speed = N (scale factor controlling playback speed of log file)
    /// - mode = r | w (log file mode. Default is read)
    /// - Eg:  "file:///path/to/logfile?speed=4"
    AgentMessenger(const std::string& url = "") throw(AgentException);

    /// Destructor
    ~AgentMessenger();

    /// \return true if the messsenger backend is properly initialised
    bool isGood() const;

    /// Subscribe a callback method of an object to receive and decode messages
    /// over a channel.
    /// This method is designed for use with C++ classes generated by lcm-gen.
    /// \param channel (input) The channel name.
    /// \param handlerMethod (input) The callback method to invoke on receiving
    ///                      a message over the channel.
    /// \param pObj (input) Class instance that the callback method must be
    ///             invoked on.
    /// \param queueLen (input) Sets the maximum number of received messages
    ///                 that can be queued up. Set this to a low number to
    ///                 reduce latency at the cost of dropped packets.
    /// \return A pointer to Subscription object. This can be used to unsubscribe
    ///         the handler later. See unsubscribe().
    template<class MessageType, class MessengerClass>
    inline lcm::Subscription* subscribe(const std::string& channel,
                   void (MessengerClass::*handlerMethod)(const lcm::ReceiveBuffer* pBuf, const std::string& channel, const MessageType* pMsg),
                   MessengerClass* pObj,
                   unsigned int queueLen = 30);

    /// Unsubscribe a message handler
    void unsubscribe(lcm::Subscription* pSubs) { return _lcm.unsubscribe(pSubs); }

    /// Encode a message and publish it over a channel.
    /// \param channel (input) The channel name
    /// \param pMsg (input) Pointer to the message. The type is created by
    ///         lcm-gen.
    template<class MessageType>
    inline bool publish(const std::string& channel, const MessageType* pMsg);

private:
    bool start();   //!< Start message handling thread
    void stop();    //!< Stop message handling thread
    void run();     //!< The handler thread function
    bool isExitFlag() const;
    void setExitFlag(bool isExit);
private:
    bool            _exitFlag;
    mutable QMutex  _flagLock;
    lcm::LCM        _lcm;
}; // AgentMessenger

//------------------------------------------------------------------------------
template<class MessageType>
bool AgentMessenger::publish(const std::string& channel, const MessageType* pMsg)
//------------------------------------------------------------------------------
{
    return (0 == _lcm.publish(channel, pMsg));
}


//------------------------------------------------------------------------------
template<class MessageType, class MessengerClass>
lcm::Subscription* AgentMessenger::subscribe(
        const std::string& channel,
        void (MessengerClass::*handlerMethod)(const lcm::ReceiveBuffer* pBuf, const std::string& channel, const MessageType* pMsg),
        MessengerClass* pObj,
        unsigned int queueLen)
//------------------------------------------------------------------------------
{
    lcm::Subscription* pSubs = NULL;
    if( (pSubs = _lcm.subscribe(channel, handlerMethod, pObj)) != NULL )
    {
        int val = pSubs->setQueueCapacity(queueLen);
        if( 0 != val )
        {
            throw MessengerException(0, "[AgentMessenger::subscribe] : Error setting message queue length");
        }
    }
    return pSubs;
}

} // Ugv1

#endif // UGV1_AGENTMESSENGER_H
