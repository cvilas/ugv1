/*
WifiNunchukJoy - Wifi enabled Nunchuk joystick

Sequence of events:
- Power up
- Set wifi connection params
  set wifi <ssid> <passphrase>
- Set remote IP of the device that is waiting on joystick info
  set remote <ip> <port>
- Send data until power down
*/


#include <WiFi.h>
#include <Wire.h>
#include "nunchuck_funcs.h"

// initialize the library instance:
WiFiClient wifiClient;

// wifi parameters
char ssid[] = "NishVil";
char pass[] = "d789a6f879";

// server parameters
IPAddress server(192,168,0,1);
int port = 1234;

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
  
  // initialise nunchuck
  nunchuck_setpowerpins();
  nunchuck_init();
  
  // connect to wifi
  connectToWiFi();
  connectToServer();
}

//-----------------------------------------------------------------
void loop() 
//-----------------------------------------------------------------
{ 
  int joyX = 0;
  int joyY = 0;
  byte butZ = 0;
  byte butC = 0;
  
  // stop if server disconnected
  if( !wifiClient.connected() )
  {
    Serial.println("Disconnected from Server. Stopping.");
    wifiClient.stop();
    while(true); // do nothing forever
  }
  
  // read nunchuck every 100 ms
  if( loopCount > 100 ) 
  {
    loopCount = 0;

    nunchuck_get_data();
    joyX = nunchuck_joyx();
    joyY = nunchuck_joyy();
    butZ = nunchuck_zbutton();
    butC = nunchuck_cbutton(); 
    
    // todo: scale data to range [-128,0,127]
  
    wifiClient.print((int)joyX, DEC);
    Serial.print((int)joyX, DEC);
  
    wifiClient.print(" ");
    Serial.print(" ");

    wifiClient.print((int)joyY, DEC);
    Serial.print((int)joyY, DEC);

    wifiClient.print(" ");
    Serial.print(" ");

    wifiClient.print((int)butZ, DEC);
    Serial.print((int)butZ, DEC);

    wifiClient.print(" ");
    Serial.print(" ");

    wifiClient.println((int)butC, DEC);    
    Serial.println((int)butC, DEC);    
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
void connectToServer()
//-----------------------------------------------------------------
{
  if( wifiClient.connect(server, port) )
  {
    Serial.print("connected to ");
    Serial.print(server);
    Serial.print(" on port ");
    Serial.println(port);
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
