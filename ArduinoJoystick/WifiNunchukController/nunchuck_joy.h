#ifndef NUNCHUCK_JOY_H
#define NUNCHUCK_JOY_H


#include "nunchuck_funcs.h"

//=========================================================================
struct NunData // wrapper for nunchuck data
//=========================================================================
{
  union
  {
    struct
    {
      int8_t h1;   // header byte 1 'J'
      int8_t h2;   // header byte 2 'S'
      int8_t jx;   // joystick x (-128 to +127)
      int8_t jy;   // joystick y (-128 to +127)
      int8_t ax;   // accelerometer x (approx 70 - 182)
      int8_t ay;   // accelerometer y (approx 65 - 173)
      int8_t btn;  // buttons
      int8_t csum; // checksum
    };
    int8_t rawData[8];
  };
};

//=========================================================================
class NunStick // wrapper class for nunchuck driver
//=========================================================================
{
 public:
  static const unsigned int Z_BTN_MAP = 0;
  static const unsigned int C_BTN_MAP = 1;
  
 public:
  NunStick() {}
  ~NunStick() {}
  
  void init();        // initialise
  void calibrate();   // hold both buttons and wiggle the stick across full range of motion
  void update();      // read the nunchuk and update internal state
  const struct NunData& getData() const { return _jsData; } // access to state
  bool isZBtn() const { return (_jsData.btn >> Z_BTN_MAP) & 1; } // convenience function. True if Z button was pressed during last update()
  bool isCBtn() const { return (_jsData.btn >> C_BTN_MAP) & 1; } // convenience function. True if C button was pressed during last update()
  
 private:
  int8_t checksum(); // compute checksum for all bytes in state
  
 private:
  struct NunData _jsData;
  uint8_t _minJX;
  uint8_t _minJY;
  uint8_t _maxJX;
  uint8_t _maxJY;
};

//-----------------------------------------------------------------
void NunStick::init()
//-----------------------------------------------------------------
{
  nunchuck_setpowerpins();
  nunchuck_init();
  
  _minJX = 0;
  _minJY = 0;
  _maxJX = 255;
  _maxJY = 255;
  
  // set header.
  _jsData.rawData[0] = 'J';
  _jsData.rawData[1] = 'S';
}

//-----------------------------------------------------------------
void NunStick::calibrate()
//-----------------------------------------------------------------
{
  uint8_t minX = 255;
  uint8_t minY = 255;
  uint8_t maxX = 0;
  uint8_t maxY = 0;
  
  // get ranges
  do
  {
    delay(10);
    nunchuck_get_data();
    
    uint8_t x = nunchuck_joyx();
    uint8_t y = nunchuck_joyy();
    
    if( x > maxX ) { maxX = x; }
    if( x < minX ) { minX = x; }
    if( y > maxY ) { maxY = y; }
    if( y < minY ) { minY = y; }
       
  } while( nunchuck_zbutton() && nunchuck_cbutton() );
  
  // set ranges. if error (exited above loop too early), set safe defaults
  ( minX < 50 ) ? (_minJX = minX) : (_minJX = 0);
  ( maxX > 200) ? (_maxJX = maxX) : (_maxJX = 255);
  ( minY < 50 ) ? (_minJY = minY) : (_minJY = 0);
  ( maxY > 200) ? (_maxJY = maxY) : (_maxJY = 255);
}

//-----------------------------------------------------------------
void NunStick::update()
//-----------------------------------------------------------------
{
    nunchuck_get_data();
    
    uint8_t x = nunchuck_joyx();
    uint8_t y = nunchuck_joyy();
    
    if( x > _maxJX ) _maxJX = x;
    if( x < _minJX ) _minJX = x;
    if( y > _maxJY ) _maxJY = y;
    if( y < _minJY ) _minJY = y;
    
    _jsData.jx = (int8_t)(((unsigned int)(x - _minJX)*255)/(_maxJX - _minJX))-128;
    _jsData.jy = (int8_t)(((unsigned int)(y - _minJY)*255)/(_maxJY - _minJY))-128;
    _jsData.ax = 0xFF & nunchuck_accelx();
    _jsData.ay = 0xFF & nunchuck_accely();
    _jsData.btn = 0;
    _jsData.btn = (nunchuck_zbutton() << Z_BTN_MAP) + (nunchuck_cbutton() << C_BTN_MAP);
    _jsData.rawData[7] = checksum();
}

//-----------------------------------------------------------------
int8_t NunStick::checksum()
//-----------------------------------------------------------------
{
  unsigned int sum = 0;
  for(int i = 0; i < 7; ++i)
  {
    sum += (unsigned int)(_jsData.rawData[i]);
  }
  return (sum&0xff);
}

#endif // NUNCHUCK_JOY_H

