//==============================================================================
// Project  : UGV1
// Module   : IO
// File     : IoBoard.h
// Brief    : IO board interface
//==============================================================================

#ifndef UGV1_IOBOARD_H
#define UGV1_IOBOARD_H

#include "IoBoardMessage_analog.h"
#include "IoBoardMessage_dio.h"
#include "IoBoardMessage_motor.h"
#include "IoBoardMessage_servo.h"
#include "io/IPort.h"

namespace Ugv1
{

/// \class IoBoard
/// \ingroup vehicle
/// \brief UGV1 IO board communication interface.
/// This class mainly provides a mechanism for passing messages back and forth
/// from the hardware. A compatible external transport mechanism is used
/// for message passing (i.e. RS232 serial port).
/// \see IoBoardMessage, IoBoardCommand, IoBoardResponse
class UGV1IOB_DLL_API IoBoard
{

public:
    /// Publically accessible status object contains the last
    /// error code and message
    Grape::Status lastError;

public:

    /// Constructor.
    /// \param transport    Communication medium between us and the hardware.
    ///                     The transport mechanism must be initialised before
    ///                     calling any methods in this class. For instance,
    ///                     is using a serial port, the port must be open
    ///                     and configured for communication with the hardware.
    ///                     See hardware manual for port settings to use.
    /// \note Response timeout period is set to 'infinite' by default.
    /// \see setResponseTimeOut.
    IoBoard(Grape::IPort& transport);

    /// Destructor.
    ~IoBoard();

    // ------------- Transport configuration ---------------

    /// \param ms   Milliseconds to wait for response from hardware. Specify
    ///             negative number for infinite wait.
    void setResponseTimeOut(int ms) { _timeoutMs = ms; }

    // ------------- Servo signals -------------------------
    bool configureServoOutPins(const SetDioServoModeCommand& message) { return send(message); }
    bool setServo(const WriteServoOutCommand& message) { return send(message); }

    // ------------- Analog I/O ----------------------------
    bool getAnalog(ReadAnalogInResponse& response);

    // ------------- Digital I/O ---------------------------
    bool configureDigitalInPins(const SetDioIoModeCommand& message) { return send(message); }
    bool setDigitalOut(const WriteDioOutCommand& message) { return send(message); }
    bool getDigitalIn(ReadDioInResponse& response);

    // ------------- Motor control --------------------------
    bool configureDriveTrain(const SetMotorParametersCommand& message) { return send(message); }
    bool configureMotorControlGains(const SetMotorPidGainsCommand& message) { return send(message); }
    bool configureMotorDriveMode(const SetMotorDriveModeCommand& message) { return send(message); }
    bool setMotorSpeed(const WriteMotorSpeedCommand& message) { return send(message); }
    bool getMotorSpeed(ReadMotorSpeedResponse& response);
    bool getMotorCurrent(ReadMotorCurrentResponse& response);
    bool setMotorPower(const WriteMotorPowerCommand& message) { return send(message); }
    bool getMotorEncoders(ReadMotorEncodersResponse& response);
    bool resetMotorEncoders();

    // ------------- RS485 comms -----------------------------
    /// \todo RS485 comms

    // ------------- i2c comms -------------------------------
    /// \todo i2c comms

    // -------------- General --------------------------------
    bool getVersion(ReadBoardVersionResponse& response);
    bool send(const IoBoardCommand& cmd, IoBoardResponse& reply);
    bool send(const IoBoardCommand& cmd);

private:
    IoBoard(const IoBoard&); // disable copy
    IoBoard& operator=(const IoBoard&); // disable assignment

private:
    Grape::IPort&   _transport;
    int             _timeoutMs; //!< wait timeout for response from hardware
}; //IoBoard

} // namespace Ugv1

#endif // UGV1_IOBOARD_H
