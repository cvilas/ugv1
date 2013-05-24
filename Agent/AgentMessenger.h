//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentMessenger.h
// Brief    : Communication backend mechanism
//==============================================================================

#ifndef UGV1_AGENTMESSENGER_H
#define	UGV1_AGENTMESSENGER_H

#include "agent_common.h"
#include <QThread>
#include <QMutex>
#include <string>

namespace Ugv1
{

/// \class AgentMessenger
/// \ingroup agent
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
    AgentMessenger(const std::string& url = "");

    /// Destructor
    ~AgentMessenger();

    /// \return true if the messsenger backend is properly initialised
    bool isGood() const;

    /// Start message processing thread
    bool start();

    /// Stop message processing thread
    void stop();

    /// Subscribe a callback method of an object to receive and decode messages
    /// over a channel. Note that messages aren't received unless CPuceThread::start()
    /// is called first.
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
    void unsubscribe(lcm::Subscription* pSubs) { return lcm_.unsubscribe(pSubs); }

    /// Encode a message and publish it over a channel. Note that messages can
    /// be published without first calling CPuceThread::start().
    /// \param channel (input) The channel name
    /// \param pMsg (input) Pointer to the message. The type is created by
    ///         lcm-gen.
    template<class MessageType>
    inline bool publish(const std::string& channel, const MessageType* pMsg);

private:
    void run();
    bool isExitFlag() const;
    void setExitFlag(bool isExit);
private:
    bool            _exitFlag;
    mutable QMutex  _flagLock;
}; // AgentMessenger


} // Ugv1

#endif // UGV1_AGENTMESSENGER_H
