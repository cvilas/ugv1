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
    _messageMap[IoBoardMessage::WRITE_DIO]              = new WriteDioOutCommand;
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

    _messageMap[IoBoardMessage::WRITE_SERVO]            = new WriteServoOutCommand;
    _messageMap[IoBoardMessage::WRITE_MOTOR_SPEED]      = new WriteMotorSpeedCommand;
    _messageMap[IoBoardMessage::WRITE_MOTOR_POWER]      = new WriteMotorPowerCommand;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::updateConfig()
//-----------------------------------------------------------------------------
{
    Ugv1::IoBoard* pIo;
    if( !pIo->configureDigitalInPins(*((SetDioIoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_IOMODE])) )
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
bool IoBoardModel::updateIO()
//-----------------------------------------------------------------------------
{
}

} // Ugv1
