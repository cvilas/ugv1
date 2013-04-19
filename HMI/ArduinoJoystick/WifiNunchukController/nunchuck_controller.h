#ifndef NUNCHUCK_CONTROLLER_H
#define NUNCHUCK_CONTROLLER_H

#include "nunchuck_joy.h"

//=========================================================================
class NunchuckController
//=========================================================================
{  
 public:
  NunchuckController(WiFiClient& transport);
  ~NunchuckController();
  
  bool init();
  bool update();
  
 private:
  void setDirectPowerMode();
  void drive(int fwd, int turn); // range -127 to +127 for each param
  bool getEncoders(int& w1, int& w2); 
  int computeWheelLeft(int f, int t);
  int computeWheelRight(int f, int t);
  
 private:
  static const uint8_t _ugvCmdSetDirectPowerMode[7]; 
  static const uint8_t _ugvCmdReadEncoders[6];

  NunStick _js;
  WiFiClient& _transport;
};

const uint8_t NunchuckController::_ugvCmdSetDirectPowerMode[7] = {0x55, 0xAA, 0x10, 0x01, 0x10, 0x11, 0x31}; 
const uint8_t NunchuckController::_ugvCmdReadEncoders[6] = {0x55, 0xAA, 0x10, 0x00, 0x12, 0x21};

//---------------------------------------------------------------------------
NunchuckController::NunchuckController(WiFiClient& transport)
//---------------------------------------------------------------------------
: _transport(transport)
{
}

//---------------------------------------------------------------------------
NunchuckController::~NunchuckController()
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
bool NunchuckController::init()
//---------------------------------------------------------------------------
{
  _js.init();
  Serial.println("Press and hold both buttons to calibrate.");
  while(!_js.isZBtn() || !_js.isCBtn())
  {
    Serial.print(".");
    _js.update();
    delay(100);
  }
  Serial.println("");
  
  Serial.println("Calibrating joystick. Move the analog stick about it's full range");
  _js.calibrate();
  Serial.println("Joystick calibration done");
  
  // read encoders to confirm we have comms
  int w1, w2;
  if( !getEncoders(w1, w2) )
  {
    return false;
  }
  
  // set ugv in direct mode
  Serial.print("Set direct power mode...");
  setDirectPowerMode();
  Serial.println("done");
  
  delay(1000);
  
  
  drive(0,0);
  
  return true;
}

//---------------------------------------------------------------------------
bool NunchuckController::update()
//---------------------------------------------------------------------------
{
  int w1, w2;
  const struct NunData& js = _js.getData(); 

  if( getEncoders(w1, w2) )
  {
    _js.update();
    drive(js.jy, js.jx);
  }
  
  return true;
}

//---------------------------------------------------------------------------
void NunchuckController::setDirectPowerMode()
//---------------------------------------------------------------------------
{
  _transport.write(_ugvCmdSetDirectPowerMode, 7);
}

//---------------------------------------------------------------------------
void NunchuckController::drive(int f, int t)
//---------------------------------------------------------------------------
{
  static int c = 0;
  Serial.print(c);
  Serial.print(" ");
  ++c;
  
  int w1 = computeWheelLeft(f,t);
  int w2 = computeWheelRight(f,t);
  
  uint8_t cmd[8];
  cmd[0] = 0x55;
  cmd[1] = 0xAA;
  cmd[2] = 0x10;
  cmd[3] = 0x02;
  cmd[4] = 0x11;
  cmd[5] = w2;
  cmd[6] = w1;
  int s = 0;
  for(int i = 0; i < 7; ++i)
  {
    s += cmd[i];
  }
  cmd[7] = s & 0xFF;
  for(int i = 0; i < 8; ++i)
  {
    Serial.print(cmd[i], HEX);
    Serial.print(" ");
  }
  Serial.println("");
  _transport.write(cmd, 8);
}

//---------------------------------------------------------------------------
int NunchuckController::computeWheelLeft(int f, int t)
//---------------------------------------------------------------------------
{
  int af = abs(f);
  int sgn = 1;
  if( af < 1)
    sgn = 1;
  else
    sgn = f/af;
  
  int d = af;
  if( t < 0 )
  {
    d += t;
  }
  
  return (((sgn*d) + 127)*200)/255;
}

//---------------------------------------------------------------------------
int NunchuckController::computeWheelRight(int f, int t)
//---------------------------------------------------------------------------
{
  int af = abs(f);
  int sgn = 1;
  if( af < 1)
    sgn = 1;
  else
    sgn = f/af;
  
  int d = af;
  if( t > 0 )
  {
    d -= t;
  }
  
  return (((sgn*d) + 127)*200)/255;
}

//---------------------------------------------------------------------------
bool NunchuckController::getEncoders(int& w1, int& w2)
//---------------------------------------------------------------------------
{
  _transport.write(_ugvCmdReadEncoders, 6);
  
  // wait for reply
  int count = 0;
  while( (_transport.available() < 1) && (count < 1000) )
  {
    delay(1);
    count++;
    Serial.println(count);
  }
  
  return (count < 1000);
}


#endif // NUNCHUCK_CONTROLLER_H
