//==============================================================================
// Project  : UGV1
// Module   : Vehicle
// File     : IoBoardModel.h
// Brief    : IoBoard state class
//==============================================================================

#ifndef UGV1_IOBOARD_MODEL_H
#define UGV1_IOBOARD_MODEL_H

#include "IoBoard.h"
#include <map>

namespace Ugv1
{

/// \todo
/// - Check encoder output and motor drive train parameters - encoder count seems to
/// change faster than expected

/// \class IoBoardModel
/// \ingroup vehicle
/// \brief IO state model
/// The class provides a state model for various inputs and outputs on the IO board.
/// State can be changed by calling set..() methods and the current state are returned
/// by get..() methods.
/// \note
/// - The IO board does not actually provide methods to query all settable states. In
/// these instances, the corresponding get..() returns the last user setting for the parameter,
/// and not the actual state. Such get methods are identified by the getSetting..() or
/// getConfigSetting...() prefix.
/// - The board does not retain settings when power cycled. On power up, reconfigure using
/// setConfig..() and call writeConfig() to ensure the state maintained by the class is
/// consistent with actual settings on the board.
/// - Calling a set...() method does not immediately set state. Call writeConfig() and
/// writeOutputs() to write them to the board.
/// - A get...() method do not return instantaneous state; they return state at the instant
/// of last call to readInputs()
class UGV1VEHICLELIB_DLL_API IoBoardModel
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
    /// \see setDioMode, getDioMode
    enum DioMode
    {
        OUTPUT_MODE,    //!< Configure pin as digital output
        INPUT_MODE,     //!< Configure pin as digital input
        SERVO_MODE,     //!< Configure pin as servo output
        DIOMODE_MAX
    };

    ///\brief Motor control modes
    /// \see setMotorDriveMode, getMotorDriveMode
    enum DriveControlMode
    {
        DIRECT_POWER_CONTROL, //!< open loop direct power control
        SPEED_CONTROL,        //!< closed loop PID speed control
        DRIVEMODE_MAX
    };

public:

    /// Constructor. Takes a reference to the IO board as the only
    /// parameter. Configures the board with default parameters.
    IoBoardModel(IoBoard& board);

    /// Destructor. Does nothing.
    ~IoBoardModel();

    // -------------- configuration ---------------

    /// Set default configuration parameters. Call writeConfig to activate
    /// the configuration by writing it to the board.
    /// - All 11 digital lines are configured as inputs
    /// - encoder ppr = 13
    /// - gear ratio = 51:1
    /// - wheel perimeter = 430 mm
    /// - motor drive mode = speed control
    /// - p,i,d gains = 10,30,1 respectively
    /// \see writeConfig
    void setConfigDefaults();

    /// Configure digital pins as input, output or servo lines.
    /// \param channel  IO pin in range 0 - 10. Note that only pins 0 - 7
    ///                 are configurable in servo mode.
    /// \param mode     Desired mode
    /// \see writeConfig
    void setConfigDioMode(unsigned int channel, DioMode mode);
    DioMode getConfigSettingDioMode(unsigned int channel);

    /// Specify the pulses per rotation for motor encoders.
    /// \see writeConfig
    void setConfigEncoderPPR(unsigned short ppr);
    unsigned short getConfigSettingEncoderPPR();

    /// Set drive train gear ratio.
    /// \param gr10 Gear ratio multiplied by 10. i.e. If the actual ratio is
    ///             64:1, set 640.
    /// \see writeConfig
    void setConfigMotorGearRatio(unsigned short gr10);
    unsigned short getConfigSettingMotorGearRatio();

    /// Set the wheel perimeter
    /// \param mm   Wheel perimeter in millimeters.
    /// \see writeConfig
    void setConfigWheelPerimeter(unsigned short mm);
    unsigned short getConfigSettingWheelPerimeter();

    /// Configure motor control in open-loop direct power mode or closed-loop
    /// speed control mode
    /// \see writeConfig
    void setConfigMotorDriveMode(DriveControlMode mode);
    DriveControlMode getConfigSettingMotorDriveMode();

    /// Set motor controller PID proportional gain
    /// \param gain    Proportional gain multiplied by 10. Range 0 - 255.
    /// \see writeConfig
    void setConfigPGain(unsigned char gain);
    unsigned char getConfigSettingPGain();

    /// Set motor controller PID integral gain
    /// \param gain    Integral gain multiplied by 10. Range 0 - 255.
    /// \see writeConfig
    void setConfigIGain(unsigned char gain);
    unsigned char getConfigSettingIGain();

    /// Set motor controller PID damping gain
    /// \param gain    Damping gain multiplied by 10. Range 0 - 255.
    /// \see writeConfig
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
    void setServoOut(unsigned int channel, unsigned char degrees, unsigned char speed=0xFF);
    unsigned char getSettingServoPosition(unsigned int channel);

    /// Set motor drive signal.
    /// \param channel Motor number. Range 0 - 1.
    /// \param cmps If in speed control mode, this is speed in cm/sec.
    ///             If in direct power mode, this is percentage power in range - 100 to 100.
    /// \see writeOutputs
    void setMotorSpeed(unsigned int channel, int cmps);
    int getSettingMotorSpeed(unsigned int channel);

    // --------------- inputs ---------------------

    /// \return State of a digital input pin
    /// \see readInputs
    bool getDigitalIn(unsigned int channel);

    /// \return State of an analog input pin (0 - 3.3 volts)
    /// \see readInputs
    double getAnalogIn(unsigned int channel);

    /// Get motor speed. The output is 0 if the motors are configured in direct power mode.
    /// \see readInputs
    int getMotorSpeed(unsigned int channel);

    /// \return motor current consumption in mA.
    /// \see readInputs
    int getMotorCurrent(unsigned int channel);

    /// \return motor encoder count
    /// \see readInputs
    long long int getMotorEncoder(unsigned int channel);

    /// \return board version information retrieved after writeConfig()
    /// \see writeConfig
    Version getBoardVersion();

    // --------------- update -----------------

    /// Write configuration to the board. None of the setConfig..() calls take effect until
    /// a call to this method.
    virtual void writeConfig() throw(VehicleException);

    /// Apply all outputs on the board, i.e. digital output pins, servo outputs and motor control.
    /// None of the set() calls take effect until calls to this method.
    virtual void writeOutputs() throw(VehicleException);

    /// Read in inputs on the IO board, i.e. digital inputs, analog inputs, motor currents, etc.
    /// Call get...() methods after this method to obtain the latest input state.
    virtual void readInputs() throw(VehicleException);

protected:

    /// Derived classes can support additional command and response types
    /// by adding them via these methods. (Eg: Messages RS485 or i2c devices)
    void addCommandMessage(IoBoardMessage::MessageID, IoBoardCommand*);
    void addResponseMessage(IoBoardMessage::MessageID, IoBoardResponse*);

private:
    void constructMessageMap();
protected:
    std::map<IoBoardMessage::MessageID, IoBoardCommand*>    _commandMap;
    std::map<IoBoardMessage::MessageID, IoBoardResponse*>   _responseMap;
    IoBoard&                                                _board;
    bool                                                    _wasMotorCmdDirFwd[2]; //!< motor direction at previous write
    bool                                                    _isMotorCmdDirFwd[2];  //!< motor direction at latest write
    bool                                                    _isMotorRespDirFwd[2]; //!< motor direction sign after read
    unsigned int                                            _encoderResidual[2];    //!< value at reset
    bool                                                    _dioCmdChanged;         //!< set if a change if commanded for io bits
    bool                                                    _servoCmdChanged;
}; // IoBoardModel

} // namespace Ugv1

#endif // UGV1_IOBOARD_MODEL_H
