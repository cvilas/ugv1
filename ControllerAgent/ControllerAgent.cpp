//==============================================================================
// Project  : UGV1
// Module   : ControllerAgent
// File     : ControllerAgent.cpp
//==============================================================================

#include "ControllerAgent.h"
#include "Ugv1Messages/JoyMessage.hpp"
#include "timing/Timer.h"
#include "AgentBus.h"
#include <iostream>
#include <sstream>

namespace Ugv1
{

//------------------------------------------------------------------------------
IAgent* createAgent(AgentBus& man)
//------------------------------------------------------------------------------
{
    return new ControllerAgent(man);
}


//==============================================================================
ControllerAgent::ControllerAgent(AgentBus& man) throw(AgentException)
//==============================================================================
    : AgentThread(man), _periodMs(MAX_PERIOD_MS), _isConfigured(false)
{
}

//------------------------------------------------------------------------------
ControllerAgent::~ControllerAgent()
//------------------------------------------------------------------------------
{
    stop();
}

//------------------------------------------------------------------------------
void ControllerAgent::configure() throw(AgentException)
//------------------------------------------------------------------------------
{
    stop();
    _isConfigured = false;

    /*
      // Relevant sections of config file for the Joystick node

        <Messenger Url="udpm://239.255.76.67:7667?ttl=1">
            <Message Channel="JOYSTICK" Type="JoyMessage"></Message>
        </Messenger>

        <JoystickAgent>
            <DevicePort>/dev/input/js0</DevicePort>
            <Publish0>JOYSTICK</Publish0>
            <UpdateIntervalMs>20</UpdateIntervalMs>
            <AxisDeadZoneRange>2000</AxisDeadZoneRange>
            <SurgeControl ControlType="AXIS" Index="1" Scale="-1"></SurgeControl>
            <YawControl ControlType="AXIS" Index="2" Scale="-1"></YawControl>
            <DeadMansHandle ControlType="BUTTON" Index="0"></DeadMansHandle>
        </JoystickAgent>

    */

    AgentBus& bus = getBus();
    QDomDocument& config = bus.getConfig();

    // look for joystick node
    QDomNodeList nodeList = config.documentElement().elementsByTagName("JoystickAgent");
    if( nodeList.isEmpty() )
    {
        throw ConfigException(0,"[JoystickAgent::configure] No joystick configuration found");
    }

    _periodMs = MAX_PERIOD_MS;
    _lcmChannel.clear();

    // first read off entries to get joystick connection info
    QDomNode pEntries = nodeList.at(0).toElement().firstChild();
    while( !pEntries.isNull() )
    {
        QDomElement peData = pEntries.toElement();
        QString tagName = peData.tagName();
        if( tagName == "DevicePort" ) { _jsPort = peData.text().toStdString(); }
        if( tagName == "Publish0" ) { _lcmChannel = peData.text().toStdString(); }
        pEntries = pEntries.nextSibling();
    }

    if( _jsPort.length() == 0 )
    {
        throw ConfigException(0, "[JoystickAgent::configure] : Port not set");
    }
    if( _lcmChannel.empty() )
    {
        throw ConfigException(0, "[JoystickAgent::configure] : Message channel not set");
    }

    if( bus.getMessageType(_lcmChannel) != Ugv1Messages::JoyMessage::getTypeName() )
    {
        std::ostringstream str;
        str << "[JoystickAgent::configure] Type listed for Channel Name "
                  << _lcmChannel << " (" << bus.getMessageType(_lcmChannel)
                  << ") does not agree with implementation ("
                  << Ugv1Messages::JoyMessage::getTypeName() << ")";
        throw ConfigException(0, str.str());
    }

    // connect to device
    if( !_pJoystick->connect(_jsPort) )
    {
        std::ostringstream str;
        str << "[JoystickAgent::configure] Unable to connect to " << _jsPort;
        throw AgentException(0, str.str());
    }

    // read off all other entries
    pEntries = nodeList.at(0).toElement().firstChild();
    while( !pEntries.isNull() )
    {
        QDomElement peData = pEntries.toElement();
        QString tagName = peData.tagName();

        if( tagName == "UpdateIntervalMs" )
        {
            int interval = abs(peData.text().toInt());
            if( (interval < 1) || (interval > MAX_PERIOD_MS) )
            {
                std::ostringstream str;
                str << "[JoystickAgent::configure] IntervalMs incorrect (range 1 - " << MAX_PERIOD_MS << ")";
                throw ConfigException(0,str.str());
            }
            _periodMs = interval;
        }

        if( tagName == "AxisDeadZoneRange" )
        {
            int deadZone = abs(peData.text().toInt());
            if( !_pJoystick->setDeadZone(deadZone) )
            {
                std::cerr << "[JoystickAgent::configure] Unable to set dead zone = " << deadZone << std::endl;
            }
        }

        if( tagName == "SurgeControl")
        {
            std::string type = peData.attribute("ControlType").toStdString();
            unsigned int index = peData.attribute("Index").toUInt();
            int scale = peData.attribute("Scale").toInt();
            _surgeCtrl.attach(type, index, scale, *_pJoystick);
        }

        if( tagName == "YawControl" )
        {
            std::string type = peData.attribute("ControlType").toStdString();
            unsigned int index = peData.attribute("Index").toUInt();
            int scale = peData.attribute("Scale").toInt();
            _yawCtrl.attach(type, index, scale, *_pJoystick);
        }

        if( tagName == "DeadMansHandle" )
        {
            std::string type = peData.attribute("ControlType").toStdString();
            unsigned int index = peData.attribute("Index").toUInt();
            int scale = peData.attribute("Scale", "1").toInt();
            _deadMansCtrl.attach(type, index, scale, *_pJoystick);
        }

        pEntries = pEntries.nextSibling();
    }

    if( !_surgeCtrl.isValid() )
    {
        throw ConfigException(0, "[JoystickAgent::configure] : Surge control not assigned");
    }
    if( !_yawCtrl.isValid() )
    {
        throw ConfigException(0, "[JoystickAgent::configure] : Yaw control not assigned");
    }
    if( !_deadMansCtrl.isValid() )
    {
        throw ConfigException(0, "[JoystickAgent::configure] : Dead Man's handle not assigned");
    }
    _isConfigured = true;

#ifdef _DEBUG
    std::cout << "[JoystickAgent::configure] Configured "
              << _pJoystick->getName() << " to publish every "
              << _periodMs << " ms on channel " << _lcmChannel
              << std::endl;
#endif

}

//------------------------------------------------------------------------------
void ControllerAgent::run()
//------------------------------------------------------------------------------
{
    Ugv1Messages::JoyMessage message;
    AgentMessenger* pMessenger = getBus().getMessenger();
    Grape::Timer timer;
    long long periodNs = _periodMs * 1000 * 1000;
    timer.start(periodNs, false);

    while( !isExitFlag() )
    {
        message.uTime = -1;
        message.rawSurgeRate = 0;
        message.rawYawRate = 0;

        // wait
        timer.timedWait(periodNs);

        // read joystick
        if( _pJoystick->update() && _isConfigured )
        {
            // translate
            const Grape::SimpleJoystick::JoystickState& state = _pJoystick->getState();
            message.uTime = 1000 * state.ms;
            message.rawSurgeRate = (_surgeCtrl.isValid() ? _surgeCtrl.value() : 0);
            message.rawYawRate = (_yawCtrl.isValid() ? _yawCtrl.value() : 0);
            message.deadMansHandle = (_deadMansCtrl.isValid() ? (_deadMansCtrl.value() != 0) : false);
        }

        // unable to read JS
        else
        {
            std::cerr << "[JoystickAgent] update failed" << std::endl;

            // attempt a reconnect
            if( _pJoystick->connect(_jsPort) )
            {
                std::cerr << "[JoystickAgent] Reconnected" << std::endl;
            }
        }

        // publish (will send zeroes if joystick update failed)
        if( pMessenger )
        {
            if( !pMessenger->publish(_lcmChannel, &message) )
            {
                std::ostringstream str;
                str << "[JoystickAgent] Publish error on channel " << _lcmChannel;
                throw AgentException(0, str.str());
            }
        }

#ifdef _DEBUG
    std::cout << "[JoystickAgent] ["
              << message.uTime << "] ("
              << message.rawSurgeRate << ", "
              << message.rawYawRate << ") "
              << (int)message.deadMansHandle << std::endl;
#endif
    } // while
}

} // namespace Ugv1
