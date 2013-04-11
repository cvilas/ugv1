//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : Controller.cpp
//==============================================================================

#include "Controller.h"

namespace Ugv1
{

//==============================================================================
Controller::Controller()
//==============================================================================
{
}

//-----------------------------------------------------------------------------
Controller::~Controller()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void IoBoardModel::constructMessageMap()
//-----------------------------------------------------------------------------
{
    _messageMap[IoBoardMessage::SET_DIO_SERVOMODE]      = new SetDioServoModeCommand;
    _messageMap[IoBoardMessage::SET_DIO_IOMODE]         = new SetDioIoModeCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_PARAM]        = new SetMotorPidGainsCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS]    = new SetMotorPidGainsCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]    = new SetMotorDriveModeCommand;

    _messageMap[IoBoardMessage::READ_DIO]               = new ReadDioInResponse;
    _messageMap[IoBoardMessage::READ_ANALOG]            = new ReadAnalogInResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_SPEED]       = new ReadMotorSpeedResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_CURRENT]     = new ReadMotorCurrentResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_ENCODERS]    = new ReadMotorEncodersResponse;
    _messageMap[IoBoardMessage::RESET_MOTOR_ENCODERS]   = new ResetMotorEncodersCommand;
    _messageMap[IoBoardMessage::READ_BOARD_VERSION]     = new ReadBoardVersionResponse;

    _messageMap[IoBoardMessage::WRITE_DIO]              = new WriteDioOutCommand;
    _messageMap[IoBoardMessage::WRITE_SERVO]            = new WriteServoOutCommand;
    _messageMap[IoBoardMessage::WRITE_MOTOR_SPEED]      = new WriteMotorSpeedCommand;
    _messageMap[IoBoardMessage::WRITE_MOTOR_POWER]      = new WriteMotorPowerCommand;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::setConfig()
//-----------------------------------------------------------------------------
{
    Ugv1::IoBoard* pIo;
    if( !pIo->configureDigitalInPins(*((SetDioIoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_IOMODE])) )
    {
        return false;
    }
    if( !pIo->configureServoOutPins(*((SetDioServoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_SERVOMODE])) )
    {
        return false;
    }
    if( !pIo->configureDriveTrain(*((SetMotorParametersCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PARAM])) )
    {
        return false;
    }
    if( !pIo->configureMotorControlGains(*((SetMotorPidGainsCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS])) )
    {
        return false;
    }
    if( !pIo->configureMotorDriveMode(*((SetMotorDriveModeCommand*)_messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])) )
    {
        return false;
    }
    if( !pIo->getVersion(*(ReadBoardVersionResponse*)_messageMap[IoBoardMessage::READ_BOARD_VERSION]))
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
bool IoBoardModel::setOutputs()
//-----------------------------------------------------------------------------
{
    Ugv1::IoBoard* pIo;

    if( !pIo->setDigitalOut( *(WriteDioOutCommand*)_messageMap[IoBoardMessage::WRITE_DIO]) )
    {
        return false;
    }
    if( !pIo->setServo(*(WriteServoOutCommand*)_messageMap[IoBoardMessage::WRITE_SERVO]) )
    {
        return false;
    }
    if( /*mode*/ )
    {
        if( !pIo->setMotorSpeed(*(WriteMotorSpeedCommand*)_messageMap[IoBoardMessage::WRITE_MOTOR_SPEED]) )
        {
            return false;
        }
    }
    else
    {
        if( !pIo->setMotorPower(*(WriteMotorPowerCommand*)_messageMap[IoBoardMessage::WRITE_MOTOR_POWER]) )
        {
                return false;
        }
    }

}

//-----------------------------------------------------------------------------
bool IoBoardModel::getInputs()
//-----------------------------------------------------------------------------
{
    Ugv1::IoBoard* pIo;

    if( !pIo->getDigitalIn(*(ReadDioInResponse*)_messageMap[IoBoardMessage::READ_DIO]) )
    {
        return false;
    }
    if( !pIo->getAnalog(*(ReadAnalogInResponse*)_messageMap[IoBoardMessage::READ_ANALOG]) )
    {
        return false;
    }
    if( !pIo->getMotorSpeed(*(ReadMotorSpeedResponse*)_messageMap[IoBoardMessage::READ_MOTOR_SPEED]))
    {
        return false;
    }
    if( !pIo->getMotorCurrent(*(ReadMotorCurrentResponse*)_messageMap[IoBoardMessage::READ_MOTOR_CURRENT]))
    {
        return false;
    }
    if( !pIo->getMotorEncoders(*(ReadMotorEncodersResponse*)_messageMap[IoBoardMessage::READ_MOTOR_ENCODERS]))
    {
        return false;
    }

    // TODO: send this command on direction change.
    _messageMap[IoBoardMessage::RESET_MOTOR_ENCODERS]   = new ResetMotorEncodersCommand;
}

} // Ugv1
