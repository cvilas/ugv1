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

//==============================================================================
IoBoardModel::IoBoardModel()
//==============================================================================
{
    constructMessageMap();
}

//-----------------------------------------------------------------------------
IoBoardModel::~IoBoardModel()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void IoBoardModel::setDioMode(unsigned int channel, IoBoardModel::DioMode mode)
//-----------------------------------------------------------------------------
{
    switch(mode)
    {
    case IoBoardModel::INPUT_MODE:
        ((SetDioIoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_IOMODE])->setModeInput(channel, true);
        ((SetDioServoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, false);
        break;
    case IoBoardModel::OUTPUT_MODE:
        ((SetDioIoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_IOMODE])->setModeOutput(channel, true);
        ((SetDioServoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, false);
        break;
    case IoBoardModel::SERVO_MODE:
    default:
        ((SetDioServoModeCommand*)_messageMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, true);
        break;
    };
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorEncoderPpr(unsigned short ppr)
//-----------------------------------------------------------------------------
{
    ((SetMotorParametersCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PARAM])->setEncoderPPR(ppr);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorGearRatio(unsigned short ratio10)
//-----------------------------------------------------------------------------
{
    ((SetMotorParametersCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PARAM])->setGearRatio(ratio10);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setWheelPerimeter(unsigned short mm)
//-----------------------------------------------------------------------------
{
    ((SetMotorParametersCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PARAM])->setWheelPerimeter(mm);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorDriveMode(DriveControlMode mode)
//-----------------------------------------------------------------------------
{
    switch(mode)
    {
    case DIRECT_POWER_CONTROL:
        ((SetMotorDriveModeCommand*)_messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->setModeDirectPower();
        break;
    case SPEED_CONTROL:
    default:
        ((SetMotorDriveModeCommand*)_messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->setModeSpeedControl();
    };
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorControlPGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    ((SetMotorPidGainsCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setProportionalGain(gain);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorControlIGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    ((SetMotorPidGainsCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setIntegralGain(gain);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorControlDGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    ((SetMotorPidGainsCommand*)_messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setDerivativeGain(gain);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setDigitalOut(unsigned int channel, bool high)
//-----------------------------------------------------------------------------
{
    ((WriteDioOutCommand*)_messageMap[IoBoardMessage::WRITE_DIO])->setChannel(channel, high);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setServoOut(unsigned int channel, unsigned char degrees, unsigned char speed)
//-----------------------------------------------------------------------------
{
    ((WriteServoOutCommand*)_messageMap[IoBoardMessage::WRITE_SERVO])->setChannel(channel, degrees, speed);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setMotorSpeed(unsigned int channel, int cmps)
//-----------------------------------------------------------------------------
{
    SetMotorDriveModeCommand* pCmd = _messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE];
    if( pCmd->isModeDirectPower() )
    {
        ((WriteMotorPowerCommand*)_messageMap[IoBoardMessage::WRITE_MOTOR_POWER])->setPower(channel, cmps);
    }
    else if( pCmd->isModePidControl() )
    {
        ((WriteMotorSpeedCommand*)_messageMap[IoBoardMessage::WRITE_MOTOR_SPEED])->setSpeed(channel, cmps);
    }
}

//-----------------------------------------------------------------------------
bool IoBoardModel::getDigitalIn(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return ((ReadDioInResponse*)_messageMap[IoBoardMessage::READ_DIO])->isHigh(channel);
}

//-----------------------------------------------------------------------------
double IoBoardModel::getAnalogIn(unsigned int channel)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
int IoBoardModel::getMotorSpeed(unsigned int channel)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
int IoBoardModel::getMotorCurrent(unsigned int channel)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
int IoBoardModel::getMotorEncoder(unsigned int channel)
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
Version IoBoardModel::getBoardVersion()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void IoBoardModel::constructMessageMap()
//-----------------------------------------------------------------------------
{
    _messageMap[IoBoardMessage::SET_DIO_SERVOMODE]      = new SetDioServoModeCommand;
    _messageMap[IoBoardMessage::SET_DIO_IOMODE]         = new SetDioIoModeCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_PARAM]        = new SetMotorParametersCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_PID_GAINS]    = new SetMotorPidGainsCommand;
    _messageMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]    = new SetMotorDriveModeCommand;
    _messageMap[IoBoardMessage::RESET_MOTOR_ENCODERS]   = new ResetMotorEncodersCommand;

    _messageMap[IoBoardMessage::READ_DIO]               = new ReadDioInResponse;
    _messageMap[IoBoardMessage::READ_ANALOG]            = new ReadAnalogInResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_SPEED]       = new ReadMotorSpeedResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_CURRENT]     = new ReadMotorCurrentResponse;
    _messageMap[IoBoardMessage::READ_MOTOR_ENCODERS]    = new ReadMotorEncodersResponse;
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
    if( 1/*mode*/ )
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
