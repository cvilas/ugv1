//==============================================================================
// Project  : UGV1
// Module   : ControllerAgent
// File     : ControllerAgent.cpp
//==============================================================================

#include "ControllerAgent.h"
#include "timing/Timer.h"
#include "AgentBus.h"
#include <QDateTime>
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
    : AgentThread(man),
      _periodMs(MAX_PERIOD_MS),
      _isConfigured(false),
      _robotModel(_serialPort),
      _pJoySubscription(NULL)
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

        <MessageBus Url="udpm://239.255.76.67:7667?ttl=1">
            <Channel Name="JOYSTICK" Type="JoyMessage"></Channel>
            <Channel Name="ODOMETRY" Type="OdometryMessage"></Channel>
        </MessageBus>

        <ControllerAgent>
            <SerialPort>/dev/ttyUSB0</SerialPort>
            <JoystickChannel>JOYSTICK</JoystickChannel>
            <OdometryChannel>ODOMETRY</OdometryChannel>
            <UpdateIntervalMs>10</UpdateIntervalMs>
        </ControllerAgent>

    */

    AgentBus& bus = getBus();
    QDomDocument& config = bus.getConfig();

    // look for joystick node
    QDomNodeList nodeList = config.documentElement().elementsByTagName("ControllerAgent");
    if( nodeList.isEmpty() )
    {
        throw ConfigException(0,"[ControllerAgent::configure] No ControllerAgent configuration found");
    }

    _periodMs = MAX_PERIOD_MS;
    _odometryChannel.clear();
    _joystickChannel.clear();

    // first read off entries to get joystick connection info
    QDomNode pEntries = nodeList.at(0).toElement().firstChild();
    while( !pEntries.isNull() )
    {
        QDomElement peData = pEntries.toElement();
        QString tagName = peData.tagName();
        if( tagName == "SerialPort" ) { _serialPort.setPortName(peData.text().toStdString()); }
        if( tagName == "JoystickChannel" ) { _joystickChannel = peData.text().toStdString(); }
        if( tagName == "OdometryChannel" ) { _odometryChannel = peData.text().toStdString(); }
        pEntries = pEntries.nextSibling();
    }

    if( _serialPort.getPortName().empty() )
    {
        throw ConfigException(0, "[ControllerAgent::configure] : Serial port not set");
    }
    if( _joystickChannel.empty() )
    {
        throw ConfigException(0, "[ControllerAgent::configure] : Joystick channel not set");
    }
    if( _odometryChannel.empty() )
    {
        throw ConfigException(0, "[ControllerAgent::configure] : Odometry channel not set");
    }

    if( bus.getMessageType(_joystickChannel) != Ugv1Messages::JoyMessage::getTypeName() )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Type listed for Channel Name "
                  << _joystickChannel << " (" << bus.getMessageType(_joystickChannel)
                  << ") does not agree with implementation ("
                  << Ugv1Messages::JoyMessage::getTypeName() << ")";
        throw ConfigException(0, str.str());
    }

    if( bus.getMessageType(_odometryChannel) != Ugv1Messages::OdometryMessage::getTypeName() )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Type listed for Channel Name "
                  << _odometryChannel << " (" << bus.getMessageType(_odometryChannel)
                  << ") does not agree with implementation ("
                  << Ugv1Messages::OdometryMessage::getTypeName() << ")";
        throw ConfigException(0, str.str());
    }

    // connect to device
    if( !_serialPort.open() )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Unable to connect to " << _serialPort.getPortName();
        throw AgentException(0, str.str());
    }
    if( !_serialPort.setBaudRate(Grape::SerialPort::B115200) )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Unable to set baud rate for " << _serialPort.getPortName();
        throw AgentException(0, str.str());
    }

    if( !_serialPort.setDataFormat(Grape::SerialPort::D8N1) )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Unable to set format for " << _serialPort.getPortName();
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
                str << "[ControllerAgent::configure] IntervalMs incorrect (range 1 - " << MAX_PERIOD_MS << ")";
                throw ConfigException(0,str.str());
            }
            _periodMs = interval;
        }

        pEntries = pEntries.nextSibling();
    }

    // configure the robot model
    _modelLock.lock();
    _robotModel.setResponseTimeOut(5000);
    _robotModel.configureIoBoard();
    _modelLock.unlock();

    // set up subscription to command channel
    _pJoySubscription = getBus().getMessenger()->subscribe(_joystickChannel, &ControllerAgent::onJoystick, this, 1);
    if( !_pJoySubscription )
    {
        std::ostringstream str;
        str << "[ControllerAgent::configure] Unable to subscribe to " << _joystickChannel << " channel";
        throw ConfigException(0,str.str());
    }

    _isConfigured = true;

#ifdef _DEBUG
    std::cout << "[ControllerAgent::configure] Configured. Publish every " << _periodMs << " ms" << std::endl;
#endif

}

//------------------------------------------------------------------------------
void ControllerAgent::stop() throw()
//------------------------------------------------------------------------------
{
    if( _pJoySubscription )
    {
        getBus().getMessenger()->unsubscribe(_pJoySubscription);
    }
    AgentThread::stop();
    _serialPort.close();
}

//------------------------------------------------------------------------------
void ControllerAgent::run() throw(AgentException)
//------------------------------------------------------------------------------
{   
    Ugv1Messages::OdometryMessage odoMsg;

    AgentMessenger* pMessenger = getBus().getMessenger();

    Grape::Timer timer;
    long long periodNs = _periodMs * 1000 * 1000;
    timer.start(periodNs, false);

    while( !isExitFlag() )
    {

        // wait
        timer.timedWait(periodNs);

        // ------------ read inputs -------------------
        _modelLock.lock();

        // update model with inputs
        _robotModel.readInputs();

        odoMsg.uTime = 1000 * QDateTime::currentMSecsSinceEpoch(); // not exactly the time when data was read, but close enough
        _robotModel.getChassisVelocity(odoMsg.translationVelocity, odoMsg.rotationVelocity);

        // update local copies of model data
        int translationVelocity = 0;
        int rotationVelocity = 0;
        _robotModel.getSettingChassisVelocity(translationVelocity, rotationVelocity);
        bool isBatteryLow = _robotModel.isBatteryLow();
        bool isBumped = _robotModel.isAnyBumperActive();

        _modelLock.unlock();

        // ------------ process commands and sensors -------------------

        // Check battery voltage. If low,
        // - disable devices that consumer power (drive motor)
        // - turn off non-essential services

        //if( isBatteryLow )
        //{
        //    // stop motors
        //    translationVelocity = 0;
        //    rotationVelocity = 0;
        //}

        //  Allow only backward motion if any bumpers are active
        if( isBumped )
        {
            if( translationVelocity > 0 ) translationVelocity = 0;
        }

        // ------------ write outputs -------------------

        _modelLock.lock();

        _robotModel.setChassisVelocity(translationVelocity,rotationVelocity);
        _robotModel.writeOutputs();

        _modelLock.unlock();

        // publish odometry
        if( pMessenger )
        {
            if( !pMessenger->publish(_odometryChannel, &odoMsg) )
            {
                std::ostringstream str;
                str << "[ControllerAgent] Publish error on channel " << _odometryChannel;
                throw AgentException(0, str.str());
            }
#ifdef _DEBUG
            std::cout << "[ControllerAgent] " << _odometryChannel << " "
                      << odoMsg.uTime << " "
                      << odoMsg.translationVelocity << " "
                      << odoMsg.rotationVelocity << std::endl;
#endif
        }

    } // while
}

//------------------------------------------------------------------------------
void ControllerAgent::onJoystick(const lcm::ReceiveBuffer* rBuf,
                               const std::string& channel,
                               const Ugv1Messages::JoyMessage* pMsg)
//------------------------------------------------------------------------------
{
    rBuf = rBuf;

    if( channel != _joystickChannel )
    {
        return;
    }


    /// \todo
    /// - scale joystick velocity inputs as appropriate
    /// - do mode selection based on joystick button state

    _commandLock.lock();

    bool isDead = (pMsg->deadMansHandle == 0);
    _commandMsg.uTime = 1000 * QDateTime::currentMSecsSinceEpoch();
    _commandMsg.desiredModeId = 0;
    _commandMsg.surgeSpeed = (isDead ? 0 : (100 * pMsg->rawSurgeRate)/32767);
    _commandMsg.yawSpeed = (isDead ? 0 : (100 * pMsg->rawYawRate)/32767);

    // apply command
    /// \todo: this needs to be in main control loop with safeguards for broken joystick agent
    _modelLock.lock();
    _robotModel.setChassisVelocity(_commandMsg.surgeSpeed, _commandMsg.yawSpeed);
    _modelLock.unlock();

    _commandLock.unlock();
/*
#ifdef _DEBUG
    std::cout << "[ControllerAgent] COMMAND "
              << _commandMsg.uTime << " "
              << _commandMsg.desiredModeId << " "
              << _commandMsg.surgeSpeed << " "
              << _commandMsg.yawSpeed << std::endl;
#endif
*/
}

} // namespace Ugv1
