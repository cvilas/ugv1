/*
WifiNunchukController - Wifi enabled Nunchuk controller

This controller directly controls the UGV through a wireless link to the 
RN-XV on its serial port.

Sequence of events:
- Power up
- Set wifi connection params
  set wifi <ssid> <passphrase>
- Set remote IP of the device that is waiting on joystick info
  set remote <ip> <port>
- Put ugv in direct power mode
- In a loop
-- Read encoders
-- Send motor drive signal based on joystick input

*/

#include <Wire.h>
#include <WiFi.h>

#include "nunchuck_controller.h"


// initialize the library instance:
WiFiClient wifiClient;
NunchuckController controller(wifiClient);

// wifi parameters
char ssid[] = "VilasPhone";
char pass[] = "";

// server parameters
IPAddress ugvAddress(192,168,43,69);
int ugvPort = 2000;

int loopCount = 0;

//-----------------------------------------------------------------
void setup() 
//-----------------------------------------------------------------
{
  // initialise serial port
  Serial.begin(9600);
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
   
  // connect to wifi
  connectToWiFi();
  connectToUgv();
  
  // initialise controller and ugv
  if( !controller.init() )
  {
    Serial.println("Unable to initialise"); 
    wifiClient.stop();
    while(true);
  }
}

//-----------------------------------------------------------------
void loop() 
//-----------------------------------------------------------------
{ 
   
  // stop if ugv disconnected
  if( !wifiClient.connected() )
  {
    Serial.println("Disconnected from UGV. Stopping.");
    wifiClient.stop();
    while(true); // do nothing forever
    //todo: try soft reset
  }
  
  // do control every 100 ms
  if( loopCount > 100 ) 
  {
    loopCount = 0;
    if( !controller.update() )
    {
      Serial.println("Unable to command UGV. Stopping.");
      wifiClient.stop();
      while(true); // do nothing forever
      //todo: try soft reset
    }
  }
  
  loopCount++;
  delay(1);
}

//-----------------------------------------------------------------
void connectToWiFi()
//-----------------------------------------------------------------
{
  int wifiStatus = WL_IDLE_STATUS;
  
  while ( wifiStatus != WL_CONNECTED) 
  { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    wifiStatus = WiFi.begin(ssid, pass);

    delay(5000);
  } 

  printWifiStatus();
}

//-----------------------------------------------------------------
void connectToUgv()
//-----------------------------------------------------------------
{
  if( wifiClient.connect(ugvAddress, ugvPort) )
  {
    Serial.print("connected to ");
    Serial.print(ugvAddress);
    Serial.print(" on port ");
    Serial.println(ugvPort);
  }
  else
  {
    Serial.println("connection failed");
    wifiClient.stop();
  }
}

//-----------------------------------------------------------------
void printWifiStatus() 
//-----------------------------------------------------------------
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("Local IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

