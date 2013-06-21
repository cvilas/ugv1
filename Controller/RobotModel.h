//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : RobotModel.h
// Brief    : UGV1 robot model
//==============================================================================

#ifndef UGV1_ROBOTMODEL_H
#define UGV1_ROBOTMODEL_H

#include "IoBoardModel.h"

namespace Ugv1
{

/// \class RobotModel
/// \ingroup controller
/// \brief UGV1 robot model.
///
/// This class extends the IoBoardModel class to implement models for kinematics,
/// on-board sensors and actuators
/// \todo
/// - implement battery monitor functions
class UGV1CONTROLLER_DLL_API RobotModel : public IoBoardModel
{
public:
    // device mappings
    static const unsigned int BUMPER_PORT_DICHANNEL = 8;
    static const unsigned int BUMPER_MID_DICHANNEL = 9;
    static const unsigned int BUMPER_STBD_DICHANNEL = 10;

    static const unsigned int MOTOR_PORT_CHANNEL = 0;
    static const unsigned int MOTOR_STBD_CHANNEL = 1;

    static const unsigned int BATTV_AICHANNEL = 0;

    // control settings
    static const unsigned short MIN_BATTERY_VOLTAGE_COUNT = 0xFFF; //todo: set this to 11v equivalent
    static const int DEFAULT_PID_PGAIN = 10;
    static const int DEFAULT_PID_IGAIN = 30;
    static const int DEFAULT_PID_DGAIN = 1;

    // kinematics
    static const int WHEEL_BASE_MM = 285;
    static const int WHEEL_PERIMETER_MM = 430;
    static const int WHEEL_GEAR_RATIO = 510;
    static const int ENCODER_PPR = 13;

public:
    RobotModel(Grape::IDataPort& transport);
    ~RobotModel();

    // -------------- bumper switches ---------------
    bool isBumperPortActive();
    bool isBumperMiddleActive();
    bool isBumperStbdActive();
    bool isAnyBumperActive();

    // -------------- battery monitor ---------------
    // todo: getBatteryLevelPercent
    unsigned short getBatteryVoltageCount();
    bool isBatteryLow();

    // -------------- kinematics ---------------
    /// Set kinematic velocity of the vehicle
    /// \param cmps Translational velocity in centi-meters/sec
    /// \param crps Rotational velocity in centi-radians/sec (resolution is about 7 centiradians/sec)
    void setChassisVelocity(int cmps, int crps);
    void getSettingChassisVelocity(int&cmps, int& crps);
    void getChassisVelocity(int&cmps, int& crps);

    /// Helper function to configure io board channels and parameters.
    void configureIoBoard() throw(ControllerException);

private:

};// RobotModel

} // namespace Ugv1

#endif // UGV1_ROBOTMODEL_H
