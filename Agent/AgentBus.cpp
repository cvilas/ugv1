//==============================================================================
// Project  : UGV1
// Module   : Agent
// File     : AgentBus.cpp
//==============================================================================

#include "AgentBus.h"
#include <sstream>
#include <QFile>

namespace Ugv1
{

//==============================================================================
AgentBus::AgentBus(const std::string& cfgFileName) throw(AgentException)
//==============================================================================
    : _pMessenger(NULL)
{
    QFile file( cfgFileName.c_str() );
    if( !file.open(QIODevice::ReadOnly) )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": Unable to open " << cfgFileName;
        throw ConfigException(0, str.str());
    }

    // parse configuration file
    bool ret = _config.setContent(&file);
    file.close();
    if( !ret )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": Unable to parse " << cfgFileName;
        throw ConfigException(0, str.str());
    }

    // run a basic check
    QDomElement docElem = _config.documentElement();
    if( docElem.tagName() != "Ugv1" )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": Incorrect root element in configuration file " << cfgFileName;
        throw ConfigException(0, str.str());
    }

    createMessenger();
}

//------------------------------------------------------------------------------
AgentBus::~AgentBus()
//------------------------------------------------------------------------------
{
    if( _pMessenger )
    {
        delete _pMessenger;
    }
}

//------------------------------------------------------------------------------
std::string AgentBus::getMessageType(const std::string& channel)
//------------------------------------------------------------------------------
{
    std::map<std::string, std::string>::const_iterator it = _channels.find(channel);
    if( it != _channels.end() )
    {
        return it->second;
    }
    return std::string("");
}

//------------------------------------------------------------------------------
void AgentBus::createMessenger() throw(AgentException)
//------------------------------------------------------------------------------
{
    // we will create these when all's well
    if( _pMessenger )
    {
        delete _pMessenger;
        _pMessenger = NULL;
    }

    _channels.clear();

    // Configuration of messenger and channels look like this...
    //
    //<!-- Message bus configuration and channels available -->
    //<MessageBus Url="udpm://239.255.76.67:7667?ttl=0">
    //    <Channel Name="JOYSTICK" Type="JoyMessage"></Channel>
    //    <Channel Name="USER_COMMAND" Type="CommandMessage"></Channel>
    //    <Channel Name="RGBCAMERA_IMAGE" Type="ImageMessage"></Channel>
    //    <Channel Name="DEPTHCAMERA_IMAGE" Type="ImageMessage"></Channel>
    //</MessageBus>

    // Get configuration for messsenger backend
    QDomNodeList messengerNodes = _config.documentElement().elementsByTagName("MessageBus");
    if( messengerNodes.isEmpty() )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": No message bus configuration found";
        throw ConfigException(0, str.str());
    }

    // take the first messenger node
    _url = messengerNodes.at(0).toElement().attribute("Url").toStdString();
    if( _url.empty() )
    {
        std::ostringstream str;
        str << __FUNCTION__ << ": Url not found in message bus configuration";
        throw ConfigException(0, str.str());
    }

    // iterate through all children
    QDomNode pEntries = messengerNodes.at(0).toElement().firstChild();
    while( !pEntries.isNull() )
    {
        QDomElement peData = pEntries.toElement();
        QString tagName = peData.tagName();
        if( tagName == "Channel" )
        {
            std::string chn(peData.attribute("Name").toStdString());
            std::string type(peData.attribute("Type").toStdString());
            if( type.length() && chn.length() )
            {
                _channels[chn] = type;
            }
        }
        //else if( tagName == "Priority" )
        //{
        //    priority = abs(peData.text().toInt());
        //}
        pEntries = pEntries.nextSibling();
    }

    // create messenger and message handling
    _pMessenger = new AgentMessenger( _url );
}

} // Ugv1

