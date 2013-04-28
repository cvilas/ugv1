//==============================================================================
// Project  : UGV1
// Module   : controller
// File     : Controller.cpp
//==============================================================================

#include "Controller.h"

namespace Ugv1
{

//==============================================================================
Controller::Controller(Grape::IPort& transport)
//==============================================================================
    : _model(transport)
{
}

//-----------------------------------------------------------------------------
Controller::~Controller()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Controller::setup() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    QObject::connect(&_loopTimer, SIGNAL(timeout()), this, SLOT(loop()));
}

//-----------------------------------------------------------------------------
void Controller::start()
//-----------------------------------------------------------------------------
{
    _loopTimer.start(DEFAULT_LOOP_UPDATE_TIME_MS);
}

//-----------------------------------------------------------------------------
void Controller::stop()
//-----------------------------------------------------------------------------
{
    _loopTimer.stop();
}

//-----------------------------------------------------------------------------
void Controller::loop() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    _model.readInputs();

    // we will be modifying these before they are applied..
    int motorSpeedPort = _model.getSettingMotorSpeed(MOTOR_PORT_CHANNEL);
    int motorSpeedStbd = _model.getSettingMotorSpeed(MOTOR_STBD_CHANNEL);

    // Check battery voltage. If low,
    // - disallow devices that consumer power (drive motor)
    // - turn of non-essential services
    // - trottle down update rate
    if( isBatteryLow() )
    {
        // stop motors
        motorSpeedPort = 0;
        motorSpeedStbd = 0;

        // reduce update rate
        _loopTimer.start(LOWBATT_LOOP_UPDATE_TIME_MS);
    }

    //  Allow only backward motion if any bumpers are active
    if( isAnyBumperActive() )
    {
        if( motorSpeedPort > 0 ) motorSpeedPort = 0;
        if( motorSpeedStbd > 0 ) motorSpeedStbd = 0;
        _model.setMotorSpeed(MOTOR_PORT_CHANNEL, motorSpeedPort);
        _model.setMotorSpeed(MOTOR_STBD_CHANNEL, motorSpeedStbd);
    }
    _model.writeOutputs();
}

//-----------------------------------------------------------------------------
bool Controller::teardown() throw()
//-----------------------------------------------------------------------------
{
    stop();
}

} // Ugv1
