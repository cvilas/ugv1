//==============================================================================
// Project  : UGV1
// Module   : TestGui
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SerialPortDlg.h"
#include <QLabel>

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    : QMainWindow(parent), _pUi(new Ui::MainWindow), _iob(_port)
{
    _pUi->setupUi(this);

    _pConnectionStatus = new QLabel;
    _pErrorInfo = new QLabel;
    _pUi->statusbar->addWidget(_pConnectionStatus);
    _pUi->statusbar->addWidget(_pErrorInfo);

    QObject::connect(_pUi->actionSerial_Port, SIGNAL(triggered()), this, SLOT(onConnectSerialPort()));

    on_setTimeoutBtn_clicked();
    updateStatusBar();
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
//-----------------------------------------------------------------------------
{
    delete _pUi;
    delete _pConnectionStatus;
}

//-----------------------------------------------------------------------------
void MainWindow::onConnectSerialPort()
//-----------------------------------------------------------------------------
{
    QDialog* pDlg = new SerialPortDlg(_port, this);
    pDlg->show();
    pDlg->exec();

    if( _port.isOpen() )
    {
        _pUi->pidControlOn->setChecked(true);
        on_pidControlOn_toggled(true);
    }
    updateStatusBar();
}

//-----------------------------------------------------------------------------
void MainWindow::updateStatusBar()
//-----------------------------------------------------------------------------
{
    bool isOpen = _port.isOpen();
    if( isOpen )
    {
        _pConnectionStatus->setText(QString("connected to ") + QString::fromStdString(_port.getPortName()));
    }
    else
    {
        _pConnectionStatus->setText("Not connected");
    }
    for(int i = 0; i < _pUi->verticalLayout->count(); ++i)
    {
        QWidget* pItem = _pUi->verticalLayout->itemAt(i)->widget();
        if( pItem )
        {
            pItem->setEnabled(isOpen);
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_setTimeoutBtn_clicked()
//-----------------------------------------------------------------------------
{
    _iob.setResponseTimeOut(_pUi->timeoutSel->value());
}

//-----------------------------------------------------------------------------
void MainWindow::on_getVersionBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadBoardVersionResponse response;
    try
    {
        _iob.getVersion(response);
        _pErrorInfo->setText("");
        QString vinfo = QString::number(response.getBoardCode()) + QString(":")
                    + QString::number(response.getBoardVersion()) + QString(":")
                    + QString::number(response.getBoardRevision());
        _pUi->versionTxt->setText(vinfo);
    }
    catch( Ugv1::VehicleException& ex)
    {
        _pErrorInfo->setText( ex.what() );
        _pUi->versionTxt->setText("error");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_getAnalogBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadAnalogInResponse response;
    try
    {
        _iob.getAnalog(response);
        _pErrorInfo->setText("");
        _pUi->analog0->setText(QString::number(response.getVolts(0)));
        _pUi->analog1->setText(QString::number(response.getVolts(1)));
        _pUi->analog2->setText(QString::number(response.getVolts(2)));
        _pUi->analog3->setText(QString::number(response.getVolts(3)));
        _pUi->analog4->setText(QString::number(response.getVolts(4)));
        _pUi->analog5->setText(QString::number(response.getVolts(5)));
        _pUi->analog6->setText(QString::number(response.getVolts(6)));
        _pUi->analog7->setText(QString::number(response.getVolts(7)));
    }
    catch( Ugv1::VehicleException& ex)
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_dioConfigBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::SetDioIoModeCommand cmd;
    cmd.setModeInput(0, _pUi->dio0->isChecked());
    cmd.setModeInput(1, _pUi->dio1->isChecked());
    cmd.setModeInput(2, _pUi->dio2->isChecked());
    cmd.setModeInput(3, _pUi->dio3->isChecked());
    cmd.setModeInput(4, _pUi->dio4->isChecked());
    cmd.setModeInput(5, _pUi->dio5->isChecked());
    cmd.setModeInput(6, _pUi->dio6->isChecked());
    cmd.setModeInput(7, _pUi->dio7->isChecked());
    cmd.setModeInput(8, _pUi->dio8->isChecked());
    cmd.setModeInput(9, _pUi->dio9->isChecked());
    cmd.setModeInput(10, _pUi->dio10->isChecked());
    try
    {
        _iob.configureDigitalInPins(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex)
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_doSetBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::WriteDioOutCommand cmd;
    cmd.setChannel(0, _pUi->do0->isChecked());
    cmd.setChannel(1, _pUi->do1->isChecked());
    cmd.setChannel(2, _pUi->do2->isChecked());
    cmd.setChannel(3, _pUi->do3->isChecked());
    cmd.setChannel(4, _pUi->do4->isChecked());
    cmd.setChannel(5, _pUi->do5->isChecked());
    cmd.setChannel(6, _pUi->do6->isChecked());
    cmd.setChannel(7, _pUi->do7->isChecked());
    cmd.setChannel(8, _pUi->do8->isChecked());
    cmd.setChannel(9, _pUi->do9->isChecked());
    cmd.setChannel(10, _pUi->do10->isChecked());
    try
    {
        _iob.setDigitalOut(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_configServoBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::SetDioServoModeCommand cmd;
    cmd.setModeServo(0, _pUi->ds0->isChecked());
    cmd.setModeServo(1, _pUi->ds1->isChecked());
    cmd.setModeServo(2, _pUi->ds2->isChecked());
    cmd.setModeServo(3, _pUi->ds3->isChecked());
    cmd.setModeServo(4, _pUi->ds4->isChecked());
    cmd.setModeServo(5, _pUi->ds5->isChecked());
    cmd.setModeServo(6, _pUi->ds6->isChecked());
    cmd.setModeServo(7, _pUi->ds7->isChecked());
    try
    {
        _iob.configureServoOutPins(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_setServoBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::WriteServoOutCommand cmd;
    cmd.setChannel(0, _pUi->s0->value()&0xFF, 0xFF);
    cmd.setChannel(1, _pUi->s1->value()&0xFF, 0xFF);
    cmd.setChannel(2, _pUi->s2->value()&0xFF, 0xFF);
    cmd.setChannel(3, _pUi->s3->value()&0xFF, 0xFF);
    cmd.setChannel(4, _pUi->s4->value()&0xFF, 0xFF);
    cmd.setChannel(5, _pUi->s5->value()&0xFF, 0xFF);
    cmd.setChannel(6, _pUi->s6->value()&0xFF, 0xFF);
    cmd.setChannel(7, _pUi->s7->value()&0xFF, 0xFF);
    try
    {
        _iob.setServo(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_getDinBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadDioInResponse response;
    try
    {
        _iob.getDigitalIn(response);
        _pErrorInfo->setText("");
        _pUi->din0->setText(QString::number(response.isHigh(0)));
        _pUi->din1->setText(QString::number(response.isHigh(1)));
        _pUi->din2->setText(QString::number(response.isHigh(2)));
        _pUi->din3->setText(QString::number(response.isHigh(3)));
        _pUi->din4->setText(QString::number(response.isHigh(4)));
        _pUi->din5->setText(QString::number(response.isHigh(5)));
        _pUi->din6->setText(QString::number(response.isHigh(6)));
        _pUi->din7->setText(QString::number(response.isHigh(7)));
        _pUi->din8->setText(QString::number(response.isHigh(8)));
        _pUi->din9->setText(QString::number(response.isHigh(9)));
        _pUi->din10->setText(QString::number(response.isHigh(10)));
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
        _pUi->din0->setText("E");
        _pUi->din1->setText("E");
        _pUi->din2->setText("E");
        _pUi->din3->setText("E");
        _pUi->din4->setText("E");
        _pUi->din5->setText("E");
        _pUi->din6->setText("E");
        _pUi->din7->setText("E");
        _pUi->din8->setText("E");
        _pUi->din9->setText("E");
        _pUi->din10->setText("E");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_setDrvTrainBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::SetMotorParametersCommand cmd;
    cmd.setEncoderPPR( _pUi->encoderPpr->value()&0xFFFF );
    cmd.setGearRatio( _pUi->gearRatio->value() & 0xFFFF);
    cmd.setWheelPerimeter( _pUi->wheelPerim->value() & 0xFFFF);
    try
    {
        _iob.configureDriveTrain(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_setPidBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::SetMotorPidGainsCommand cmd;
    cmd.setProportionalGain( _pUi->gainP->value()&0xFF );
    cmd.setDerivativeGain( _pUi->gainD->value() & 0xFF );
    cmd.setIntegralGain( _pUi->gainI->value() & 0xFF );
    try
    {
        _iob.configureMotorControlGains(cmd);
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_resetEncBtn_clicked()
//-----------------------------------------------------------------------------
{
    try
    {
        _iob.resetMotorEncoders();
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_getEncBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorEncodersResponse response;
    try
    {
        _iob.getMotorEncoders(response);
        _pErrorInfo->setText("");
        _pUi->encoder0->setText( QString::number(response.getEncoder(0)) );
        _pUi->encoder1->setText( QString::number(response.getEncoder(1)) );
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
        _pUi->encoder0->setText( "Error" );
        _pUi->encoder1->setText( "Error" );
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_pidControlOn_toggled(bool checked)
//-----------------------------------------------------------------------------
{
    Ugv1::SetMotorDriveModeCommand cmd;

    if( checked )
        cmd.setModeSpeedControl();
    else
        cmd.setModeDirectPower();

    try
    {
        _iob.configureMotorDriveMode(cmd);
        _pErrorInfo->setText("");
        _pUi->motor1Slider->setValue(0);
        _pUi->motor2Slider->setValue(0);
        _motorSpeedCmd.setSpeed(0,0);
        _motorSpeedCmd.setSpeed(1,0);
        _motorPowerCmd.setPower(0,0);
        _motorPowerCmd.setPower(1,0);
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }

}

//-----------------------------------------------------------------------------
void MainWindow::on_motor1Slider_sliderMoved(int position)
//-----------------------------------------------------------------------------
{
    try
    {
        if( _pUi->pidControlOn->isChecked() )
        {
            _motorSpeedCmd.setSpeed(0, position);
            _iob.setMotorSpeed(_motorSpeedCmd);
        }
        else
        {
            _motorPowerCmd.setPower(0, position);
            _iob.setMotorPower(_motorPowerCmd);
        }
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }

    on_getSpeedBtn_clicked();
    on_getCurrentBtn_clicked();
}

//-----------------------------------------------------------------------------
void MainWindow::on_motor2Slider_sliderMoved(int position)
//-----------------------------------------------------------------------------
{
    try
    {
        if( _pUi->pidControlOn->isChecked() )
        {
            _motorSpeedCmd.setSpeed(1, position);
            _iob.setMotorSpeed(_motorSpeedCmd);
        }
        else
        {
            _motorPowerCmd.setPower(1, position);
            _iob.setMotorPower(_motorPowerCmd);
        }
        _pErrorInfo->setText("");
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
    }

    on_getSpeedBtn_clicked();
    on_getCurrentBtn_clicked();
}

//-----------------------------------------------------------------------------
void MainWindow::on_getCurrentBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorCurrentResponse response;
    try
    {
        _iob.getMotorCurrent(response);
        _pErrorInfo->setText("");
        _pUi->motor1Current->setText(QString::number(response.getMotorCurrent(0)));
        _pUi->motor2Current->setText(QString::number(response.getMotorCurrent(1)));
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
        _pUi->motor1Speed->setText("Error");
        _pUi->motor2Speed->setText("Error");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_getSpeedBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadMotorSpeedResponse response;
    try
    {
        _iob.getMotorSpeed(response);
        _pErrorInfo->setText("");
        _pUi->motor1Speed->setText(QString::number(response.getMotorSpeed(0)));
        _pUi->motor2Speed->setText(QString::number(response.getMotorSpeed(1)));
    }
    catch( Ugv1::VehicleException& ex )
    {
        _pErrorInfo->setText( ex.what() );
        _pUi->motor1Speed->setText("Error");
        _pUi->motor2Speed->setText("Error");
    }
}
