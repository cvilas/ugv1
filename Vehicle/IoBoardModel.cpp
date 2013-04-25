//==============================================================================
// Project  : UGV1
// Module   : IoBoard
// File     : IoBoardModel.cpp
//==============================================================================

#include "IoBoardModel.h"

namespace Ugv1
{

//==============================================================================
IoBoardModel::IoBoardModel(IoBoard& board)
//==============================================================================
    : _board(board), _dioCmdChanged(true), _servoCmdChanged(true)
{
    for(int i = 0; i < 2; ++i)
    {
        _wasMotorCmdDirFwd[i] = true;
        _isMotorCmdDirFwd[i] = true;
        _isMotorRespDirFwd[i] = true;
        _encoderResidual[i] = 0;
    }

    constructMessageMap();
    setConfigDefaults();
}

//-----------------------------------------------------------------------------
IoBoardModel::~IoBoardModel()
//-----------------------------------------------------------------------------
{}

//-----------------------------------------------------------------------------
void IoBoardModel::setConfigDefaults()
//-----------------------------------------------------------------------------
{
    /// - All 11 digital lines are configured as inputs
    /// - encoder ppr = 13
    /// - gear ratio = 51:1
    /// - wheel perimeter = 430 mm
    /// - motor drive mode = speed control
    /// - p,i,d gains = 10,30,1

    for(unsigned int i = 0; i < 11; ++i)
    {
        setConfigDioMode(i, IoBoardModel::INPUT_MODE);
    }
    setConfigEncoderPPR(13);
    setConfigMotorGearRatio(510);
    setConfigWheelPerimeter(430);
    setConfigMotorDriveMode(IoBoardModel::SPEED_CONTROL);
    setConfigPGain(10);
    setConfigIGain(30);
    setConfigDGain(1);
}

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
void IoBoardModel::setServoOut(unsigned int channel, unsigned char degrees, unsigned char speed)
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
void IoBoardModel::setMotorSpeed(unsigned int channel, int cmps)
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
}

//-----------------------------------------------------------------------------
int IoBoardModel::getSettingMotorSpeed(unsigned int channel)
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
int IoBoardModel::getMotorSpeed(unsigned int channel)
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
void IoBoardModel::addCommandMessage(IoBoardMessage::MessageID id, IoBoardCommand* pCmd)
//-----------------------------------------------------------------------------
{
    _commandMap[id] = pCmd;
}

//-----------------------------------------------------------------------------
void IoBoardModel::addResponseMessage(IoBoardMessage::MessageID id, IoBoardResponse* pResp)
//-----------------------------------------------------------------------------
{
    _responseMap[id] = pResp;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::writeConfig()
//-----------------------------------------------------------------------------
{
    if( !_board.send(*_commandMap[IoBoardMessage::SET_DIO_IOMODE]) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(SET_DIO_IOMODE)" << std::endl;
        return false;
    }
    if( !_board.send(*_commandMap[IoBoardMessage::SET_DIO_SERVOMODE]) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(SET_DIO_SERVOMODE)" << std::endl;
        return false;
    }
    if( !_board.send(*_commandMap[IoBoardMessage::SET_MOTOR_PARAM]) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(SET_MOTOR_PARAM)" << std::endl;
        return false;
    }
    if( !_board.send(*_commandMap[IoBoardMessage::SET_MOTOR_PID_GAINS]) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(SET_MOTOR_PID_GAINS)" << std::endl;
        return false;
    }
    if( !_board.send(*_commandMap[IoBoardMessage::SET_MOTOR_DRIVEMODE]) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(SET_MOTOR_DRIVEMODE)" << std::endl;
        return false;
    }
    if( !_board.getVersion( *dynamic_cast<ReadBoardVersionResponse*>(_responseMap[IoBoardMessage::READ_BOARD_VERSION])) )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(READ_BOARD_VERSION)" << std::endl;
        return false;
    }

    // motor controller mode may have changed. reset encoders
    for(int i = 0; i < 2; ++i)
    {
        _wasMotorCmdDirFwd[i] = true;
        _isMotorCmdDirFwd[i] = true;
        _isMotorRespDirFwd[i] = true;
        _encoderResidual[i] = 0;
    }
    if( !_board.resetMotorEncoders() )
    {
        lastError.set(-1) << "[IoBoardModel::writeConfig] Error in send(RESET_MOTOR_ENCODERS)" << std::endl;
        return false;
    }
    return true;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::writeOutputs()
//-----------------------------------------------------------------------------
{
    if( _dioCmdChanged )
    {
        if( !_board.send( *_commandMap[IoBoardMessage::WRITE_DIO]) )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_DIO)" << std::endl;
            return false;
        }
        _dioCmdChanged = false;
    }

    if( _servoCmdChanged )
    {
        if( !_board.send(*_commandMap[IoBoardMessage::WRITE_SERVO]) )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_SERVO)" << std::endl;
            return false;
        }
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
        _isMotorCmdDirFwd[i] = (getSettingMotorSpeed(i) >= 0);
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
            if( !_board.send(WriteMotorSpeedCommand()) )
            {
                lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_MOTOR_SPEED = 0)" << std::endl;
                return false;
            }
        }
        else
        {
            if( !_board.send(WriteMotorPowerCommand()) )
            {
                lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_MOTOR_POWER = 0)" << std::endl;
                return false;
            }
        }

        // wait
        Grape::milliSleep(500);

        // reset encoders
        if( !_board.resetMotorEncoders() )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(RESET_ENCODERS)" << std::endl;
            return false;
        }

        // get residials
        Ugv1::ReadMotorEncodersResponse encoders;
        if( !_board.getMotorEncoders(encoders) )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(READ_ENCODERS)" << std::endl;
            return false;
        }
        _encoderResidual[0] = encoders.getEncoder(0);
        _encoderResidual[1] = encoders.getEncoder(1);
    }

    // go again
    if( pModeCmd->isModeSpeedControl() )
    {
        if( !_board.send(*_commandMap[IoBoardMessage::WRITE_MOTOR_SPEED]) )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_MOTOR_SPEED)" << std::endl;
            return false;
        }
    }
    else
    {
        if( !_board.send(*_commandMap[IoBoardMessage::WRITE_MOTOR_POWER]) )
        {
            lastError.set(-1) << "[IoBoardModel::writeOutputs] Error in send(WRITE_MOTOR_POWER)" << std::endl;
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::readInputs()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadDioInResponse* pDioResp = dynamic_cast<ReadDioInResponse*>(_responseMap[IoBoardMessage::READ_DIO]);
    if( !_board.getDigitalIn(*pDioResp) )
    {
        lastError.set(-1) << "[IoBoardModel::readInputs] Error in send(READ_DIO)" << std::endl;
        return false;
    }
    if( !pDioResp->isValid() )
    {
        // throw exception here
    }

    if( !_board.getAnalog(*dynamic_cast<ReadAnalogInResponse*>(_responseMap[IoBoardMessage::READ_ANALOG])) )
    {
        lastError.set(-1) << "[IoBoardModel::readInputs] Error in send(READ_ANALOG)" << std::endl;
        return false;
    }
    if( !_board.getMotorSpeed(*dynamic_cast<ReadMotorSpeedResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_SPEED])))
    {
        lastError.set(-1) << "[IoBoardModel::readInputs] Error in send(READ_MOTOR_SPEED)" << std::endl;
        return false;
    }
    if( !_board.getMotorCurrent(*dynamic_cast<ReadMotorCurrentResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_CURRENT])))
    {
        lastError.set(-1) << "[IoBoardModel::readInputs] Error in send(READ_MOTOR_CURRENT)" << std::endl;
        return false;
    }
    if( !_board.getMotorEncoders(*dynamic_cast<ReadMotorEncodersResponse*>(_responseMap[IoBoardMessage::READ_MOTOR_ENCODERS])))
    {
        lastError.set(-1) << "[IoBoardModel::readInputs] Error in send(READ_MOTOR_ENCODERS)" << std::endl;
        return false;
    }
    // update motor directions since last write
    for(int i = 0; i < 2; ++i)
    {
        _isMotorRespDirFwd[i] = _isMotorCmdDirFwd[i];
    }

    return true;
}

//-----------------------------------------------------------------------------
bool IoBoardModel::verifyAllResponses()
//-----------------------------------------------------------------------------
{
    std::map<IoBoardMessage::MessageID, IoBoardResponse*>::iterator it = _responseMap.begin();
    while( it != _responseMap.end() )
    {
        if( !(it->second)->isValid() )
        {
            lastError.set(-1) << "[IoBoardModel::verifyAllResponses] Invalid response. Message ID " << (it->second)->getExpectedId() << std::endl;
            return false;
        }
        ++it;
    }
    return true;
}

} // Ugv1
