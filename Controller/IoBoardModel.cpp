//==============================================================================
// Project  : UGV1
// Module   : Controller
// File     : IoBoardModel.cpp
//==============================================================================

#include "IoBoardModel.h"
#include "timing/grapetiming_common.h"

namespace Ugv1
{

//==============================================================================
IoBoardModel::IoBoardModel(Grape::IDataPort& transport)
//==============================================================================
    : _dioCmdChanged(true),
      _servoCmdChanged(true),
      _speedCmdChanged(true),
      _dioCfgChanged(true),
      _pidGainsChanged(true),
      _driveParamsChanged(true),
      _driveModeChanged(true),
      _board(transport)
{
    for(int i = 0; i < 2; ++i)
    {
        _wasMotorCmdDirFwd[i] = true;
        _isMotorCmdDirFwd[i] = true;
        _isMotorRespDirFwd[i] = true;
        _encoderResidual[i] = 0;
    }

    constructMessageMap();
}

//-----------------------------------------------------------------------------
IoBoardModel::~IoBoardModel()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigDioMode(unsigned int channel, IoBoardModel::DioMode mode)
//-----------------------------------------------------------------------------
{
    switch(mode)
    {
    case IoBoardModel::INPUT_MODE:
        dynamic_cast<SetDioIoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_IOMODE])->setModeInput(channel, true);
        dynamic_cast<SetDioServoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, false);
        break;
    case IoBoardModel::OUTPUT_MODE:
        dynamic_cast<SetDioIoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_IOMODE])->setModeOutput(channel, true);
        dynamic_cast<SetDioServoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, false);
        break;
    case IoBoardModel::SERVO_MODE:
    default:
        dynamic_cast<SetDioServoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_SERVOMODE])->setModeServo(channel, true);
        break;
    };
    _dioCfgChanged = true;
}

//-----------------------------------------------------------------------------
IoBoardModel::DioMode IoBoardModel::getConfigSettingDioMode(unsigned int channel)
//-----------------------------------------------------------------------------
{
    if( dynamic_cast<SetDioServoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_SERVOMODE])->isModeServo(channel) )
    {
        return IoBoardModel::SERVO_MODE;
    }
    else if( dynamic_cast<SetDioIoModeCommand*>(_commandMap[IoBoardMessage::SET_DIO_IOMODE])->isModeInput(channel) )
    {
        return IoBoardModel::INPUT_MODE;
    }
    else
    {
        return IoBoardModel::OUTPUT_MODE;
    }
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigEncoderPPR(unsigned short ppr)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->setEncoderPPR(ppr);
    _driveParamsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned short IoBoardModel::getConfigSettingEncoderPPR()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->getEncoderPPR();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigMotorGearRatio(unsigned short ratio10)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->setGearRatio(ratio10);
    _driveParamsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned short IoBoardModel::getConfigSettingMotorGearRatio()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->getGearRatio();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigWheelPerimeter(unsigned short mm)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->setWheelPerimeter(mm);
    _driveParamsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned short IoBoardModel::getConfigSettingWheelPerimeter()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorParametersCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PARAM])->getWheelPerimeter();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigMotorDriveMode(DriveControlMode mode)
//-----------------------------------------------------------------------------
{
    switch(mode)
    {
    case DIRECT_POWER_CONTROL:
        dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->setModeDirectPower();
        break;
    case SPEED_CONTROL:
    default:
        dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->setModeSpeedControl();
    };
    _driveModeChanged = true;
}

//-----------------------------------------------------------------------------
IoBoardModel::DriveControlMode IoBoardModel::getConfigSettingMotorDriveMode()
//-----------------------------------------------------------------------------
{
    if( dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->isModeDirectPower() )
    {
        return IoBoardModel::DIRECT_POWER_CONTROL;
    }
    else if ( dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE])->isModeSpeedControl() )
    {
        return IoBoardModel::SPEED_CONTROL;
    }
    else
    {
        return IoBoardModel::DRIVEMODE_MAX;
    }
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigPGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setProportionalGain(gain);
    _pidGainsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned char IoBoardModel::getConfigSettingPGain()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->getProportionalGain();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigIGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setIntegralGain(gain);
    _pidGainsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned char IoBoardModel::getConfigSettingIGain()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->getIntegralGain();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigDGain(unsigned char gain)
//-----------------------------------------------------------------------------
{
    dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->setDerivativeGain(gain);
    _pidGainsChanged = true;
}

//-----------------------------------------------------------------------------
unsigned char IoBoardModel::getConfigSettingDGain()
//-----------------------------------------------------------------------------
{
    return dynamic_cast<SetMotorPidGainsCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS])->getDerivativeGain();
}

//-----------------------------------------------------------------------------
void IoBoardModel::setDigitalOut(unsigned int channel, bool high)
//-----------------------------------------------------------------------------
{
    dynamic_cast<WriteDioOutCommand*>(_commandMap[IoBoardMessage::WRITE_DIO])->setChannel(channel, high);
    _dioCmdChanged = true;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::getSettingDigitalOut(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<WriteDioOutCommand*>(_commandMap[IoBoardMessage::WRITE_DIO])->getChannel(channel);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setServoPosition(unsigned int channel, unsigned char degrees, unsigned char speed)
//-----------------------------------------------------------------------------
{
    dynamic_cast<WriteServoOutCommand*>(_commandMap[IoBoardMessage::WRITE_SERVO])->setChannel(channel, degrees, speed);
    _servoCmdChanged = true;
}

//-----------------------------------------------------------------------------
unsigned char IoBoardModel::getSettingServoPosition(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<WriteServoOutCommand*>(_commandMap[IoBoardMessage::WRITE_SERVO])->getPosition(channel);
}

//-----------------------------------------------------------------------------
void IoBoardModel::setWheelSpeed(unsigned int channel, int cmps)
//-----------------------------------------------------------------------------
{
    SetMotorDriveModeCommand* pModeCmd = dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]);
    if( pModeCmd->isModeDirectPower() )
    {
        dynamic_cast<WriteMotorPowerCommand*>(_commandMap[IoBoardMessage::WRITE_MOTOR_POWER])->setPower(channel, cmps);
    }
    else if( pModeCmd->isModeSpeedControl() )
    {
        dynamic_cast<WriteMotorSpeedCommand*>(_commandMap[IoBoardMessage::WRITE_MOTOR_SPEED])->setSpeed(channel, cmps);
    }
    _speedCmdChanged = true;
}

//-----------------------------------------------------------------------------
int IoBoardModel::getSettingWheelSpeed(unsigned int channel)
//-----------------------------------------------------------------------------
{
    SetMotorDriveModeCommand* pModeCmd = dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]);
    if( pModeCmd->isModeDirectPower() )
    {
        return dynamic_cast<WriteMotorPowerCommand*>(_commandMap[IoBoardMessage::WRITE_MOTOR_POWER])->getPower(channel);
    }
    else if( pModeCmd->isModeSpeedControl() )
    {
        return dynamic_cast<WriteMotorSpeedCommand*>(_commandMap[IoBoardMessage::WRITE_MOTOR_SPEED])->getSpeed(channel);
    }
    return 0;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::getDigitalIn(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<ReadDioInResponse*>(_responseMap[IoBoardMessage::READ_DIO])->isHigh(channel);
}

//-----------------------------------------------------------------------------
double IoBoardModel::getAnalogIn(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<ReadAnalogInResponse*>(_responseMap[IoBoardMessage::READ_ANALOG])->getVolts(channel);
}

//-----------------------------------------------------------------------------
unsigned short IoBoardModel::getAnalogCountIn(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<ReadAnalogInResponse*>(_responseMap[IoBoardMessage::READ_ANALOG])->getCount(channel);
}

//-----------------------------------------------------------------------------
int IoBoardModel::getWheelSpeed(unsigned int channel)
//-----------------------------------------------------------------------------
{
    int sign = ((_isMotorRespDirFwd[channel])?(1):(-1));
    return sign * dynamic_cast<ReadMotorSpeedResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_SPEED])->getMotorSpeed(channel);
}

//-----------------------------------------------------------------------------
int IoBoardModel::getMotorCurrent(unsigned int channel)
//-----------------------------------------------------------------------------
{
    return dynamic_cast<ReadMotorCurrentResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_CURRENT])->getMotorCurrent(channel);
}

//-----------------------------------------------------------------------------
long long int IoBoardModel::getMotorEncoder(unsigned int channel)
//-----------------------------------------------------------------------------
{
    long long int sign = ((_isMotorRespDirFwd[channel])?(1):(-1));
    return sign * (-(long long int)(_encoderResidual[channel]) + dynamic_cast<ReadMotorEncodersResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_ENCODERS])->getEncoder(channel));
}

//-----------------------------------------------------------------------------
IoBoardModel::Version IoBoardModel::getBoardVersion()
//-----------------------------------------------------------------------------
{
    Version version;
    ReadBoardVersionResponse* pResp = dynamic_cast<ReadBoardVersionResponse*>(_responseMap[IoBoardMessage::READ_BOARD_VERSION]);
    version.code = pResp->getBoardCode();
    version.version = pResp->getBoardVersion();
    version.revision = pResp->getBoardRevision();
    version.reserved = 0;
    return version;
}

//-----------------------------------------------------------------------------
void IoBoardModel::constructMessageMap()
//-----------------------------------------------------------------------------
{
    _commandMap[IoBoardMessage::SET_DIO_SERVOMODE]      = new SetDioServoModeCommand;
    _commandMap[IoBoardMessage::SET_DIO_IOMODE]         = new SetDioIoModeCommand;
    _commandMap[IoBoardMessage::SET_MOTOR_PARAM]        = new SetMotorParametersCommand;
    _commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS]    = new SetMotorPidGainsCommand;
    _commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]    = new SetMotorDriveModeCommand;

    _commandMap[IoBoardMessage::WRITE_DIO]              = new WriteDioOutCommand;
    _commandMap[IoBoardMessage::WRITE_SERVO]            = new WriteServoOutCommand;
    _commandMap[IoBoardMessage::WRITE_MOTOR_SPEED]      = new WriteMotorSpeedCommand;
    _commandMap[IoBoardMessage::WRITE_MOTOR_POWER]      = new WriteMotorPowerCommand;

    _responseMap[IoBoardMessage::READ_DIO]               = new ReadDioInResponse;
    _responseMap[IoBoardMessage::READ_ANALOG]            = new ReadAnalogInResponse;
    _responseMap[IoBoardMessage::READ_MOTOR_SPEED]       = new ReadMotorSpeedResponse;
    _responseMap[IoBoardMessage::READ_MOTOR_CURRENT]     = new ReadMotorCurrentResponse;
    _responseMap[IoBoardMessage::READ_MOTOR_ENCODERS]    = new ReadMotorEncodersResponse;
    _responseMap[IoBoardMessage::READ_BOARD_VERSION]     = new ReadBoardVersionResponse;
}

//-----------------------------------------------------------------------------
void IoBoardModel::readBoardVersion() throw(ControllerException)
//-----------------------------------------------------------------------------
{    
    _board.getVersion( *dynamic_cast<ReadBoardVersionResponse*>(_responseMap[IoBoardMessage::READ_BOARD_VERSION]));
}

//-----------------------------------------------------------------------------
void IoBoardModel::writeOutputs(bool forceAll) throw(ControllerException)
//-----------------------------------------------------------------------------
{
    // configuration

    if( _dioCfgChanged || forceAll )
    {
        _board.send(*_commandMap[IoBoardMessage::SET_DIO_IOMODE]);
        _board.send(*_commandMap[IoBoardMessage::SET_DIO_SERVOMODE]);
        _dioCfgChanged = false;
    }

    if( _driveParamsChanged || forceAll )
    {
        _board.send(*_commandMap[IoBoardMessage::SET_MOTOR_PARAM]);
        _driveParamsChanged = false;
    }

    if( _pidGainsChanged || forceAll )
    {
        _board.send(*_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS]);
        _pidGainsChanged = false;
    }

    if( _driveModeChanged || forceAll )
    {
        _board.send(*_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]);
        for(int i = 0; i < 2; ++i)
        {
            _wasMotorCmdDirFwd[i] = true;
            _isMotorCmdDirFwd[i] = true;
            _isMotorRespDirFwd[i] = true;
            _encoderResidual[i] = 0;
        }
        _board.resetMotorEncoders();
        _driveModeChanged = false;
    }

    // state change commands

    if( _dioCmdChanged || forceAll )
    {
        _board.send( *_commandMap[IoBoardMessage::WRITE_DIO]);
        _dioCmdChanged = false;
    }

    if( _servoCmdChanged || forceAll )
    {
        _board.send(*_commandMap[IoBoardMessage::WRITE_SERVO]);
        _servoCmdChanged = false;
    }

    // if motor direction of motion is to be changed, then
    // - stop motor
    // - reset encoder
    // - set motor desired speed

    // is any motor changing direction?
    bool doEncoderReset = false;
    for(int i = 0;i < 2; ++i)
    {
        _isMotorCmdDirFwd[i] = (getSettingWheelSpeed(i) >= 0);
        if( _isMotorCmdDirFwd[i] != _wasMotorCmdDirFwd[i] )
        {
            doEncoderReset = true;
        }
        _wasMotorCmdDirFwd[i] = _isMotorCmdDirFwd[i];
    }

    SetMotorDriveModeCommand* pModeCmd = dynamic_cast<SetMotorDriveModeCommand*>(_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]);

    if( doEncoderReset )
    {
        // stop first
        if( pModeCmd->isModeSpeedControl() )
        {
            _board.send(WriteMotorSpeedCommand());
        }
        else
        {
            _board.send(WriteMotorPowerCommand());
        }

        // wait
        Grape::milliSleep(100);

        // reset encoders
        _board.resetMotorEncoders();

        // get residials
        Ugv1::ReadMotorEncodersResponse encoders;
        _board.getMotorEncoders(encoders);

        _encoderResidual[0] = encoders.getEncoder(0);
        _encoderResidual[1] = encoders.getEncoder(1);
    }

    // go again
    if( _speedCmdChanged || forceAll )
    {
        if( pModeCmd->isModeSpeedControl() )
        {
            _board.send(*_commandMap[IoBoardMessage::WRITE_MOTOR_SPEED]);
        }
        else
        {
            _board.send(*_commandMap[IoBoardMessage::WRITE_MOTOR_POWER]);
        }
        _speedCmdChanged = false;
    }
}

//-----------------------------------------------------------------------------
void IoBoardModel::readInputs() throw(ControllerException)
//-----------------------------------------------------------------------------
{
    _board.getDigitalIn(*dynamic_cast<ReadDioInResponse*>(_responseMap[IoBoardMessage::READ_DIO]));
    _board.getAnalog(*dynamic_cast<ReadAnalogInResponse*>(_responseMap[IoBoardMessage::READ_ANALOG]));
    _board.getMotorSpeed(*dynamic_cast<ReadMotorSpeedResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_SPEED]));
    _board.getMotorCurrent(*dynamic_cast<ReadMotorCurrentResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_CURRENT]));
    _board.getMotorEncoders(*dynamic_cast<ReadMotorEncodersResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_ENCODERS]));

	// update motor directions since last write
    for(int i = 0; i < 2; ++i)
    {
        _isMotorRespDirFwd[i] = _isMotorCmdDirFwd[i];
    }
}

} // Ugv1
