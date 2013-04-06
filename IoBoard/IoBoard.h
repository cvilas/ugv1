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
class UGV1_DLL_API IoBoard
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
    void configureServoOutPins(const SetDioServoModeCommand& message);
    void setServo(const WriteServoOutCommand& message);

    // ------------- Analog I/O ----------------------------
    bool getAnalog(ReadAnalogInResponse& response);

    // ------------- Digital I/O ---------------------------
    void configureDigitalOutPins(const SetDioIoModeCommand& message);
    void setDigitalOut(const WriteDioOutCommand& message);
    bool getDigitalIn(ReadDioInResponse& response);

    // ------------- Motor control --------------------------
    void configureDriveTrain(const SetMotorParametersCommand& message);
    void configureMotorControlGains(const SetMotorPidGainsCommand& message);
    void configureMotorControlMode(const SetMotorDriveModeCommand& message);
    void setMotorSpeed(const WriteMotorSpeedCommand& message);
    bool getMotorSpeed(ReadMotorSpeedResponse& response);
    bool getMotorCurrent(ReadMotorCurrentResponse& response);
    void setMotorPower(const WriteMotorPowerCommand& message);
    bool getMotorEncoders(ReadMotorEncodersResponse& response);
    void resetMotorEncoders();

    // ------------- RS485 comms -----------------------------
    /// \todo RS485 comms

    // ------------- i2c comms -------------------------------
    /// \todo i2c comms

    // -------------- General --------------------------------
    bool getVersion(ReadBoardVersionResponse& response);
    bool send(const IoBoardCommand& cmd, IoBoardResponse& reply);
    bool send(const IoBoardCommand& cmd);
    inline std::string getLastError(int& errorCode);

private:
    IoBoard(const IoBoard&); // disable copy
    IoBoard& operator=(const IoBoard&); // disable assignment
    inline std::ostringstream& setError(int errorCode);

private:
    Grape::IPort&   _transport;
    int             _timeoutMs; //!< wait timeout for response from hardware
    std::ostringstream  _errorStream;
    int                 _errorCode;
}; //IoBoard

//-----------------------------------------------------------------------------
std::string IoBoard::getLastError(int& errorCode)
//-----------------------------------------------------------------------------
{
    _errorCode = errorCode;
    return _errorStream.str();
}

//-----------------------------------------------------------------------------
std::ostringstream& IoBoard::setError(int errorCode)
//-----------------------------------------------------------------------------
{
    _errorCode = errorCode;
    _errorStream.clear();
    _errorStream.str(std::string());
    return _errorStream;
}

} // namespace Ugv1

#endif // UGV1_IOBOARD_H
