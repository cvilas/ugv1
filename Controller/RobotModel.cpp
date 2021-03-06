//==============================================================================
// Project  : UGV1
// Module   : controller
// File     : RobotModel.cpp
//==============================================================================

#include "RobotModel.h"

namespace Ugv1
{

//==============================================================================
RobotModel::RobotModel(Grape::IDataPort& transport)
//==============================================================================
    : IoBoardModel(transport)
{
    configureIoBoard();
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
    return (getDigitalIn(BUMPER_PORT_DICHANNEL) == 0);
}

//-----------------------------------------------------------------------------
bool RobotModel::isBumperMiddleActive()
//-----------------------------------------------------------------------------
{
    return (getDigitalIn(BUMPER_MID_DICHANNEL) == 0);
}

//-----------------------------------------------------------------------------
bool RobotModel::isBumperStbdActive()
//-----------------------------------------------------------------------------
{
    return (getDigitalIn(BUMPER_STBD_DICHANNEL) == 0);
}

//-----------------------------------------------------------------------------
bool RobotModel::isAnyBumperActive()
//-----------------------------------------------------------------------------
{
    return isBumperPortActive() || isBumperMiddleActive() || isBumperStbdActive();
}

//-----------------------------------------------------------------------------
unsigned short RobotModel::getBatteryVoltageCount()
//-----------------------------------------------------------------------------
{
    return getAnalogCountIn(BATTV_AICHANNEL);
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

    setWheelSpeed(MOTOR_PORT_CHANNEL, vl);
    setWheelSpeed(MOTOR_STBD_CHANNEL, vr);
}

//-----------------------------------------------------------------------------
void RobotModel::getSettingChassisVelocity(int&cmps, int& crps)
//-----------------------------------------------------------------------------
{
    int vl = getSettingWheelSpeed(MOTOR_PORT_CHANNEL);
    int vr = getSettingWheelSpeed(MOTOR_STBD_CHANNEL);
    cmps = (vl + vr)/2;
    crps = (1000 * (vr - vl))/WHEEL_BASE_MM;
}

//-----------------------------------------------------------------------------
void RobotModel::getChassisVelocity(int& cmps, int& crps)
//-----------------------------------------------------------------------------
{
    int vl = getWheelSpeed(MOTOR_PORT_CHANNEL);
    int vr = getWheelSpeed(MOTOR_STBD_CHANNEL);
    cmps = (vl + vr)/2;
    crps = (1000 * (vr - vl))/WHEEL_BASE_MM;
}

//-----------------------------------------------------------------------------
void RobotModel::configureIoBoard() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    setConfigDioMode(BUMPER_PORT_DICHANNEL, Ugv1::IoBoardModel::INPUT_MODE);
    setConfigDioMode(BUMPER_STBD_DICHANNEL, Ugv1::IoBoardModel::INPUT_MODE);
    setConfigDioMode(BUMPER_MID_DICHANNEL, Ugv1::IoBoardModel::INPUT_MODE);

    setConfigEncoderPPR(ENCODER_PPR);
    setConfigMotorGearRatio(WHEEL_GEAR_RATIO);
    setConfigWheelPerimeter(WHEEL_PERIMETER_MM);
    setConfigPGain(DEFAULT_PID_PGAIN);
    setConfigIGain(DEFAULT_PID_IGAIN);
    setConfigDGain(DEFAULT_PID_DGAIN);
    setConfigMotorDriveMode(IoBoardModel::SPEED_CONTROL);
}

} // Ugv1
