//==============================================================================
// Project  : UGV1
// Module   : controller
// File     : RobotModel.cpp
//==============================================================================

#include "RobotModel.h"

namespace Ugv1
{

//==============================================================================
RobotModel::RobotModel(IoBoardModel& model)
//==============================================================================
    : _model(model)
{
}

//-----------------------------------------------------------------------------
RobotModel::~RobotModel()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
bool RobotModel::isBumperPortActive()
//-----------------------------------------------------------------------------
{
    bool b = false;
    _lock.lock();
    b = _model.getDigitalIn(BUMPER_PORT_DICHANNEL);
    _lock.unlock();
    return b;
}

//-----------------------------------------------------------------------------
bool RobotModel::isBumperMiddleActive()
//-----------------------------------------------------------------------------
{
    bool b = false;
    _lock.lock();
    b = _model.getDigitalIn(BUMPER_MID_DICHANNEL);
    _lock.unlock();
    return b;}

//-----------------------------------------------------------------------------
bool RobotModel::isBumperStbdActive()
//-----------------------------------------------------------------------------
{
    bool b = false;
    _lock.lock();
    b = _model.getDigitalIn(BUMPER_STBD_DICHANNEL);
    _lock.unlock();
    return b;
}

//-----------------------------------------------------------------------------
bool RobotModel::isAnyBumperActive()
//-----------------------------------------------------------------------------
{
    bool b = false;
    _lock.lock();
    b = _model.getDigitalIn(BUMPER_PORT_DICHANNEL)
            || _model.getDigitalIn(BUMPER_MID_DICHANNEL)
            || _model.getDigitalIn(BUMPER_STBD_DICHANNEL);
    _lock.unlock();
    return b;
}

//-----------------------------------------------------------------------------
unsigned short RobotModel::getBatteryVoltageCount()
//-----------------------------------------------------------------------------
{
    unsigned short v;
    _lock.lock();
    v = _model.getAnalogCountIn(BATTV_AICHANNEL);
    _lock.unlock();
    return v;
}

//-----------------------------------------------------------------------------
bool RobotModel::isBatteryLow()
//-----------------------------------------------------------------------------
{
    return getBatteryVoltageCount() < MIN_BATTERY_VOLTAGE_COUNT;
}

//-----------------------------------------------------------------------------
void RobotModel::setChassisVelocity(int cmps, int crps)
//-----------------------------------------------------------------------------
{
    int wr = (crps * WHEEL_BASE_MM)/2000;
    int vr = cmps + wr;
    int vl = cmps - wr;

    _lock.lock();
    _model.setMotorSpeed(0, vl);
    _model.setMotorSpeed(1, vr);
    _lock.unlock();
}

//-----------------------------------------------------------------------------
void RobotModel::setup() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    _lock.lock();
    _model.setConfigEncoderPPR(ENCODER_PPR);
    _model.setConfigMotorGearRatio(WHEEL_GEAR_RATIO);
    _model.setConfigWheelPerimeter(WHEEL_PERIMETER_MM);
    _model.setConfigPGain(DEFAULT_PID_PGAIN);
    _model.setConfigIGain(DEFAULT_PID_IGAIN);
    _model.setConfigDGain(DEFAULT_PID_DGAIN);
    _model.setConfigMotorDriveMode(IoBoardModel::SPEED_CONTROL);
    _lock.unlock();
}

//-----------------------------------------------------------------------------
void RobotModel::loop() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    _lock.lock();
    _model.readInputs();
    _lock.unlock();

    // Make local copy IO that will be modified. Will apply these atomically
    // to the board in the end
    int motorSpeedPort = _model.getSettingMotorSpeed(MOTOR_PORT_CHANNEL);
    int motorSpeedStbd = _model.getSettingMotorSpeed(MOTOR_STBD_CHANNEL);

    // Check battery voltage. If low,
    // - disallow devices that consumer power (drive motor)
    // - turn of non-essential services
    if( isBatteryLow() )
    {
        // stop motors
        motorSpeedPort = 0;
        motorSpeedStbd = 0;
    }

    //  Allow only backward motion if any bumpers are active
    if( isAnyBumperActive() )
    {
        if( motorSpeedPort > 0 ) motorSpeedPort = 0;
        if( motorSpeedStbd > 0 ) motorSpeedStbd = 0;
    }

    // lock and update outputs
    _lock.lock();
    _model.setMotorSpeed(MOTOR_PORT_CHANNEL, motorSpeedPort);
    _model.setMotorSpeed(MOTOR_STBD_CHANNEL, motorSpeedStbd);
    _model.writeOutputs();
    _lock.unlock();
}

//-----------------------------------------------------------------------------
void RobotModel::teardown() throw()
//-----------------------------------------------------------------------------
{
}

} // Ugv1
