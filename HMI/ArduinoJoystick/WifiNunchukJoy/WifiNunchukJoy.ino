/*
WifiNunchukJoy - Wifi enabled Nunchuk joystick

Data format:
['J', 'S', x (0-0xff), y (0-0xff), bz, bc, checksum] 

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
IPAddress server(172,16,17,118);
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
  //connectToServer();
}

//-----------------------------------------------------------------
void loop() 
//-----------------------------------------------------------------
{ 
  uint8_t jdata[7] = {'J', 'S', 0, 0, 0, 0, 0};
   
  /*
  // stop if server disconnected
  if( !wifiClient.connected() )
  {
    Serial.println("Disconnected from Server. Stopping.");
    wifiClient.stop();
    while(true); // do nothing forever
    //todo: try soft reset
  }*/
  
  // read nunchuck every 100 ms
  if( loopCount > 100 ) 
  {
    loopCount = 0;

    nunchuck_get_data();
    jdata[2] = 0xFF & nunchuck_joyx();
    jdata[3] = 0xFF & nunchuck_joyy();
    jdata[4] = 0xFF & nunchuck_zbutton();
    jdata[5] = 0xFF & nunchuck_cbutton(); 
    jdata[6] = checksum(jdata, 6);
     
    //wifiClient.write(jdata,7);   
    
    for(int i = 0; i < 7;++i)
    {
      Serial.print(jdata[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
  
  loopCount++;
  delay(1);
}

//-----------------------------------------------------------------
uint8_t checksum(uint8_t* jdata, int n)
//-----------------------------------------------------------------
{
  int sum = 0;
  for(int i = 0; i < n; ++i)
  {
    sum += (int)(jdata[i]);
  }
  return (sum&0xff);
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

