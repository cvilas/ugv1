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
  void setMotorPower(int w1, int w2);
  bool getEncoders(int& w1, int& w2); 
  
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
  // init joystick
  // read encoders to confirm we have comms
  // set ugv in direct mode
}

//---------------------------------------------------------------------------
bool NunchuckController::update()
//---------------------------------------------------------------------------
{
  // read nunchuck
  
  // consruct drive command
  
  // send drive command
}

//---------------------------------------------------------------------------
void NunchuckController::setDirectPowerMode()
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
void NunchuckController::setMotorPower(int w1, int w2)
//---------------------------------------------------------------------------
{
}

//---------------------------------------------------------------------------
bool NunchuckController::getEncoders(int& w1, int& w2)
//---------------------------------------------------------------------------
{
}


#endif // NUNCHUCK_CONTROLLER_H
