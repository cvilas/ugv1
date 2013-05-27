//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : RobotModel.h
// Brief    : UGV1 robot model
//==============================================================================

#ifndef UGV1_ROBOTMODEL_H
#define UGV1_ROBOTMODEL_H

#include "IoBoardModel.h"
#include <QTimer>

namespace Ugv1
{

/// \class RobotModel
/// \ingroup controller
/// \brief UGV1 robot model.
/*
 * Design notes:
 * setup() to configure devices and attach them to IO
 * loop() called at a specific frequency in a separate thread.
 * before entering loop(), io input state is updated and device states are updated as configured
 * after exiting loop(), io outputs get written from devices
 * cleanup() to shutdown bits before exit
 */
class UGV1CONTROLLER_DLL_API RobotModel : public QObject
{
    Q_OBJECT
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
    static const int DEFAULT_LOOP_UPDATE_TIME_MS = 10;
    static const int LOWBATT_LOOP_UPDATE_TIME_MS = 1000;

public:
    RobotModel(Grape::IPort& transport);
    ~RobotModel();

    // bumper switches
    bool isBumperPortActive() { return _model.getDigitalIn(BUMPER_PORT_DICHANNEL); }
    bool isBumperMiddleActive() { return _model.getDigitalIn(BUMPER_MID_DICHANNEL); }
    bool isBumperStbdActive() { return _model.getDigitalIn(BUMPER_STBD_DICHANNEL); }
    bool isAnyBumperActive() { return isBumperPortActive() || isBumperMiddleActive() || isBumperStbdActive(); }

    // battery monitor
    // todo: getBatteryLevelPercent
    unsigned short getBatteryVoltageCount() { return _model.getAnalogCountIn(BATTV_AICHANNEL); }
    bool isBatteryLow() { return getBatteryVoltageCount() < MIN_BATTERY_VOLTAGE_COUNT; }

    void start();
    void stop();

public slots:
    void setup() throw(ControllerException);
    void loop() throw(ControllerException);
    void teardown() throw(/*nothing*/);
private:
    IoBoardModel    _model;
    QTimer          _loopTimer;
};// RobotModel

} // namespace Ugv1

#endif // UGV1_CONTROLLER_H
