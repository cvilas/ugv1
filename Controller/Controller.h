//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : Controller.h
// Brief    : UGV1 controller class
//==============================================================================

#ifndef UGV1_CONTROLLER_H
#define UGV1_CONTROLLER_H

#include "IoBoard/IoBoard.h"
#include <map>

namespace Ugv1
{

class UGV1_DLL_API IoBoardModel
{
public:

    /// \brief Version information
    struct Version
    {
        union
        {
            struct
            {
                unsigned int code:8;
                unsigned int version:8;
                unsigned int revision:8;
                unsigned int reserved:8;
            };
            unsigned int value;
        };
    };

    /// \brief Digital IO configuration options
    enum DioMode
    {
        OUTPUT_MODE,    //!< Configure pin as digital output
        INPUT_MODE,     //!< Configure pin as digital input
        SERVO_MODE      //!< Configure pin as servo output
    };

    ///\brief Motor control modes
    enum DriveControlMode
    {
        DIRECT_POWER_CONTROL, //!< open loop direct power control
        SPEED_CONTROL         //!< closed loop PID speed control
    };

public:
    IoBoardModel();
    ~IoBoardModel();

    // -------------- configuration ---------------
    void setDioMode(unsigned int channel, DioMode mode);
    void setMotorEncoderPpr(unsigned short ppr);
    void setMotorGearRatio(unsigned short ratio10);
    void setWheelPerimeter(unsigned short mm);
    void setMotorDriveMode(DriveControlMode mode);
    void setMotorControlPGain(unsigned char gain);
    void setMotorControlIGain(unsigned char gain);
    void setMotorControlDGain(unsigned char gain);

    // --------------- outputs --------------------
    void setDigitalOut(unsigned int channel, bool high);
    void setServoOut(unsigned int channel, unsigned char degrees, unsigned char speed);
    void setMotorSpeed(unsigned int channel, int cmps);

    // --------------- inputs ---------------------
    bool getDigitalIn(unsigned int channel);
    double getAnalogIn(unsigned int channel);
    int getMotorSpeed(unsigned int channel);
    int getMotorCurrent(unsigned int channel);
    int getMotorEncoder(unsigned int channel);
    Version getBoardVersion();

    // --------------- update -----------------
    virtual bool setConfig();
    virtual bool setOutputs();
    virtual bool getInputs();

protected:
    virtual void constructMessageMap();
private:
    std::map<IoBoardMessage::MessageID, IoBoardMessage*> _messageMap;
};

/// \class Controller
/// \ingroup vehicle
/// \brief UGV1 controller class.
/*
 * Design notes:
 * Robot will have one object derived from controller class
 * setup() to configure devices and attach them to IO
 * loop() called at a specific frequency in a separate thread.
 * before entering loop(), io input state is updated and device states are updated as configured
 * after exiting loop(), io outputs get written from devices
 * cleanup() to shutdown bits before exit
 */
class UGV1_DLL_API Controller
{
public:
    Controller();
    ~Controller();
    virtual void setup() = 0;
    virtual void loop() = 0;
    virtual void cleanup() = 0;
private:
};// Controller

} // namespace Ugv1

#endif // UGV1_CONTROLLER_H
