//==============================================================================
// Project  : UGV1
// Module   : Controller
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
/// \ingroup controller
/// \brief UGV1 IO board communication interface.
/// This class mainly provides a mechanism for passing messages back and forth
/// from the hardware. A compatible external transport mechanism is used
/// for message passing (i.e. RS232 serial port).
/// \see IoBoardMessage, IoBoardCommand, IoBoardResponse
class UGV1CONTROLLER_DLL_API IoBoard
{

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
    void configureServoOutPins(const SetDioServoModeCommand& message) { send(message); }
    void setServo(const WriteServoOutCommand& message) { send(message); }

    // ------------- Analog I/O ----------------------------
    void getAnalog(ReadAnalogInResponse& response);

    // ------------- Digital I/O ---------------------------
    void configureDigitalInPins(const SetDioIoModeCommand& message) { send(message); }
    void setDigitalOut(const WriteDioOutCommand& message) { send(message); }
    void getDigitalIn(ReadDioInResponse& response);

    // ------------- Motor control --------------------------
    void configureDriveTrain(const SetMotorParametersCommand& message) { send(message); }
    void configureMotorControlGains(const SetMotorPidGainsCommand& message) { send(message); }
    void configureMotorDriveMode(const SetMotorDriveModeCommand& message) { send(message); }
    void setMotorSpeed(const WriteMotorSpeedCommand& message) { send(message); }
    void getMotorSpeed(ReadMotorSpeedResponse& response);
    void getMotorCurrent(ReadMotorCurrentResponse& response);
    void setMotorPower(const WriteMotorPowerCommand& message) { send(message); }
    void getMotorEncoders(ReadMotorEncodersResponse& response);
    void resetMotorEncoders();

    // ------------- RS485 comms -----------------------------
    /// \todo RS485 comms

    // ------------- i2c comms -------------------------------
    /// \todo i2c comms

    // -------------- General --------------------------------
    void getVersion(ReadBoardVersionResponse& response);

	/// Send a command and receive a response
	/// On error, the following exceptions are thrown
	/// - IoWaitException
	/// - IoWriteException
	/// - IoReadException
    void send(const IoBoardCommand& cmd, IoBoardResponse& reply) throw(ControllerException);

	/// Send command that generate a response from the IoBoard.
	/// On error the following exceptions are thrown
	/// - IoWaitException
	/// - IoWriteException
    void send(const IoBoardCommand& cmd) throw(ControllerException);

private:
    IoBoard(const IoBoard&); // disable copy
    IoBoard& operator=(const IoBoard&); // disable assignment

private:
    Grape::IPort&   _transport;
    int             _timeoutMs; //!< wait timeout for response from hardware
}; //IoBoard

} // namespace Ugv1

#endif // UGV1_IOBOARD_H
