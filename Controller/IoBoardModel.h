//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardModel.h
// Brief    : IoBoard state class
//==============================================================================

#ifndef UGV1_IOBOARD_MODEL_H
#define UGV1_IOBOARD_MODEL_H

#include "IoBoard.h"
#include <map>

namespace Ugv1
{

/// \class IoBoardModel
/// \ingroup controller
/// \brief IO state model
/// The class provides a state model for various inputs and outputs on the IO board.
/// State can be changed by calling set..() methods and the current state are returned
/// by get..() methods.
/// \note
/// - The IO board does not actually provide methods to query all settable states. In
/// these instances, the corresponding get..() returns the last user setting for the parameter,
/// and not the actual state. Such get methods are identified by the getSetting..() or
/// getConfigSetting...() prefix.
/// - The board does not retain settings when power cycled. On power up, reconfigure by
/// first calling setConfigDefault() followed by other setConfig..() as necessary to ensure
/// the state maintained by the class is consistent with actual settings on the board.
/// - Calling a set...() method does not immediately set state. Call writeOutputs() to write
/// them to the board.
/// - A get...() method do not return instantaneous state; they return state at the instant
/// of last call to readInputs()
/// \todo
/// - Check encoder output and motor drive train parameters - encoder count seems to
/// change faster than expected
class UGV1CONTROLLER_DLL_API IoBoardModel
{
public:

    /// \brief Version information
    /// \see getBoardVersion
    typedef struct _Version
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
    }Version;

    /// \brief Digital IO configuration options
    /// \see setConfigDioMode, getConfigSettingDioMode
    enum DioMode
    {
        OUTPUT_MODE,    //!< Configure pin as digital output
        INPUT_MODE,     //!< Configure pin as digital input
        SERVO_MODE,     //!< Configure pin as servo output
        DIOMODE_MAX
    };

    ///\brief Motor control modes
    /// \see setConfigMotorDriveMode, getConfigSettingMotorDriveMode
    enum DriveControlMode
    {
        DIRECT_POWER_CONTROL, //!< open loop direct power control
        SPEED_CONTROL,        //!< closed loop PID speed control
        DRIVEMODE_MAX
    };

public:

    /// Constructor initialises the state with default parameters.
    IoBoardModel(Grape::IDataPort& transport);

    /// Destructor. Does nothing.
    ~IoBoardModel();

    // -------------- configuration ---------------

    /// \param ms   Milliseconds to wait for response from hardware. Specify
    ///             negative number for infinite wait.
    void setResponseTimeOut(int ms) { _board.setResponseTimeOut(ms); }

    /// Configure digital pins as input, output or servo lines.
    /// \param channel  IO pin in range 0 - 10. Note that only pins 0 - 7
    ///                 are configurable in servo mode.
    /// \param mode     Desired mode
    /// \see writeOutputs
    void setConfigDioMode(unsigned int channel, DioMode mode);
    DioMode getConfigSettingDioMode(unsigned int channel);

    /// Specify the pulses per rotation for motor encoders.
    /// \see writeOutputs
    void setConfigEncoderPPR(unsigned short ppr);
    unsigned short getConfigSettingEncoderPPR();

    /// Set drive train gear ratio.
    /// \param gr10 Gear ratio multiplied by 10. i.e. If the actual ratio is
    ///             64:1, set 640.
    /// \see writeOutputs
    void setConfigMotorGearRatio(unsigned short gr10);
    unsigned short getConfigSettingMotorGearRatio();

    /// Set the wheel perimeter
    /// \param mm   Wheel perimeter in millimeters.
    /// \see writeOutputs
    void setConfigWheelPerimeter(unsigned short mm);
    unsigned short getConfigSettingWheelPerimeter();

    /// Configure motor control in open-loop direct power mode or closed-loop
    /// speed control mode
    /// \see writeOutputs
    void setConfigMotorDriveMode(DriveControlMode mode);
    DriveControlMode getConfigSettingMotorDriveMode();

    /// Set motor controller PID proportional gain
    /// \param gain    Proportional gain multiplied by 10. Range 0 - 255.
    /// \see writeOutputs
    void setConfigPGain(unsigned char gain);
    unsigned char getConfigSettingPGain();

    /// Set motor controller PID integral gain
    /// \param gain    Integral gain multiplied by 10. Range 0 - 255.
    /// \see writeOutputs
    void setConfigIGain(unsigned char gain);
    unsigned char getConfigSettingIGain();

    /// Set motor controller PID damping gain
    /// \param gain    Damping gain multiplied by 10. Range 0 - 255.
    /// \see writeOutputs
    void setConfigDGain(unsigned char gain);
    unsigned char getConfigSettingDGain();

    // --------------- outputs --------------------

    /// Set digital output pin for channels 0 - 10.
    /// \see writeOutputs
    void setDigitalOut(unsigned int channel, bool high);
    bool getSettingDigitalOut(unsigned int channel);

    /// Set position and speed for a servo channel
    /// \param channel  Servo channel in range 0 - 7.
    /// \param degrees Position value in range 0 to 180. Center position is 90.
    /// \param speed    Speed value from 0 to 0xFF.
    /// \see writeOutputs
    void setServoPosition(unsigned int channel, unsigned char degrees, unsigned char speed=0xFF);
    unsigned char getSettingServoPosition(unsigned int channel);

    /// Set wheel drive signal.
    /// \param channel Motor number. Range 0 - 1.
    /// \param cmps If in speed control mode, this is speed in cm/sec.
    ///             If in direct power mode, this is percentage power in range - 100 to 100.
    /// \see writeOutputs
    void setWheelSpeed(unsigned int channel, int cmps);
    int getSettingWheelSpeed(unsigned int channel);

    // --------------- inputs ---------------------

    /// \return State of a digital input pin
    /// \see readInputs
    bool getDigitalIn(unsigned int channel);

    /// \return State of an analog input pin (0 - 3.3 volts)
    /// \see readInputs
    double getAnalogIn(unsigned int channel);

    /// \return Raw ADC count at an analog input pin. 12 bits long. 0x0000 maps to 0V and 0x0FFF maps to 3.3V
    /// \see readInputs
    unsigned short getAnalogCountIn(unsigned int channel);

    /// Get wheel speed in cm/s. The output is 0 if the motors are configured in direct power mode.
    /// \see readInputs
    int getWheelSpeed(unsigned int channel);

    /// \return motor current consumption in mA.
    /// \see readInputs
    int getMotorCurrent(unsigned int channel);

    /// \return motor encoder count
    /// \see readInputs
    long long int getMotorEncoder(unsigned int channel);

    /// \return board version information retrieved after readBoardVersion()
    /// \see readBoardVersion
    Version getBoardVersion();

    // --------------- update -----------------

    /// Read version from the board. Also serves as a comms test.
    virtual void readBoardVersion() throw(ControllerException);

    /// Apply any configuration or outputs changed since last time a call to
    /// this method was made.
    /// None of the set() calls take effect until this method is called.
    /// \param forceAll When true, all configuration parameters and outputs are forced
    ///                 to the board, whether they have changed or not. This is useful
    ///                 when calling writeOutputs for the first time in a loop in order
    ///                 force all IO on the board to a known state.
    virtual void writeOutputs(bool forceAll = false) throw(ControllerException);

    /// Read in inputs on the IO board, i.e. digital inputs, analog inputs, motor currents, etc.
    /// Call get...() methods after this method to obtain the latest input state.
    virtual void readInputs() throw(ControllerException);

protected:

    virtual void constructMessageMap();

protected:
    bool    _wasMotorCmdDirFwd[2]; //!< motor direction at previous write
    bool    _isMotorCmdDirFwd[2];  //!< motor direction at latest write
    bool    _isMotorRespDirFwd[2]; //!< motor direction sign after read
    bool    _dioCmdChanged;         //!< set if a change if commanded for io bits
    bool    _servoCmdChanged;
    bool    _speedCmdChanged;
    bool    _dioCfgChanged;
    bool    _pidGainsChanged;
    bool    _driveParamsChanged;
    bool    _driveModeChanged;
    IoBoard _board;
    unsigned int _encoderResidual[2];    //!< value at reset
    std::map<IoBoardMessage::MessageID, IoBoardCommand*>    _commandMap;
    std::map<IoBoardMessage::MessageID, IoBoardResponse*>   _responseMap;
}; // IoBoardModel

} // namespace Ugv1

#endif // UGV1_IOBOARD_MODEL_H
