//==============================================================================
// Project  : UGV1
// Module   : TestIobModel
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SerialPortDlg.h"
#include <QLabel>

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    : QMainWindow(parent), _pUi(new Ui::MainWindow), _iob(_port), _model(_iob)
{
    _pUi->setupUi(this);

    _pConnectionStatus = new QLabel;
    _pErrorInfo = new QLabel;
    _pUi->statusBar->addWidget(_pConnectionStatus);
    _pUi->statusBar->addWidget(_pErrorInfo);

    _timer.setSingleShot(false);

    _iob.setResponseTimeOut(5000);

    QObject::connect(_pUi->actionSerial_Port, SIGNAL(triggered()), this, SLOT(onConnectSerialPort()));
    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(onTimer()));

    initialise();

    updateStatusBar();
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
//-----------------------------------------------------------------------------
{
    delete _pUi;
}

//-----------------------------------------------------------------------------
void MainWindow::onConnectSerialPort()
//-----------------------------------------------------------------------------
{
    QDialog* pDlg = new SerialPortDlg(_port, this);
    pDlg->show();
    pDlg->exec();

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
    for(int i = 0; i < _pUi->gridLayout_5->count(); ++i)
    {
        QWidget* pItem = _pUi->gridLayout_5->itemAt(i)->widget();
        if( pItem )
        {
            pItem->setEnabled(isOpen);
        }
    }
}

//-----------------------------------------------------------------------------
void MainWindow::initialise()
//-----------------------------------------------------------------------------
{
    // set all values from the ui to model
    // fix problem with motor control
    error!
}

//-----------------------------------------------------------------------------
void MainWindow::on_configBtn_clicked()
//-----------------------------------------------------------------------------
{
    if( _model.setConfig() )
    {
        _pErrorInfo->setText("Configure succeeded");
    }
    else
    {
        _pErrorInfo->setText("Configure failed");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_startStopBtn_clicked()
//-----------------------------------------------------------------------------
{
    if( _timer.isActive() )
    {
        _timer.stop();
        _pUi->msconf->setEnabled(true);
        _pUi->configBtn->setEnabled(true);
        _pUi->startStopBtn->setText("Start");
    }
    else
    {
        _pUi->msconf->setEnabled(false);
        _pUi->configBtn->setEnabled(false);
        _timer.start( _pUi->msconf->value() );
        _pUi->startStopBtn->setText("Stop");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::onTimer()
//-----------------------------------------------------------------------------
{
    if( !_model.readInputs() )
    {
        _pErrorInfo->setText("Input Read Error");
    }
    else
    {
        _pErrorInfo->setText("");
    }

    _pUi->analog0->setText(QString::number(_model.getAnalogIn(0)));
    _pUi->analog1->setText(QString::number(_model.getAnalogIn(1)));
    _pUi->analog2->setText(QString::number(_model.getAnalogIn(2)));
    _pUi->analog3->setText(QString::number(_model.getAnalogIn(3)));
    _pUi->analog4->setText(QString::number(_model.getAnalogIn(4)));
    _pUi->analog5->setText(QString::number(_model.getAnalogIn(5)));
    _pUi->analog6->setText(QString::number(_model.getAnalogIn(6)));
    _pUi->analog7->setText(QString::number(_model.getAnalogIn(7)));

    _pUi->din0->setText(QString::number(_model.getDigitalIn(0)));
    _pUi->din1->setText(QString::number(_model.getDigitalIn(1)));
    _pUi->din2->setText(QString::number(_model.getDigitalIn(2)));
    _pUi->din3->setText(QString::number(_model.getDigitalIn(3)));
    _pUi->din4->setText(QString::number(_model.getDigitalIn(4)));
    _pUi->din5->setText(QString::number(_model.getDigitalIn(5)));
    _pUi->din6->setText(QString::number(_model.getDigitalIn(6)));
    _pUi->din7->setText(QString::number(_model.getDigitalIn(7)));
    _pUi->din8->setText(QString::number(_model.getDigitalIn(8)));
    _pUi->din9->setText(QString::number(_model.getDigitalIn(9)));
    _pUi->din10->setText(QString::number(_model.getDigitalIn(10)));

    _pUi->motor0Current->setText(QString::number(_model.getMotorCurrent(0)));
    _pUi->motor1Current->setText(QString::number(_model.getMotorCurrent(1)));

    _pUi->motor0Speed->setText(QString::number(_model.getMotorSpeed(0)));
    _pUi->motor1Speed->setText(QString::number(_model.getMotorSpeed(1)));

    _pUi->encoder0->setText(QString::number(_model.getMotorEncoder(0)));
    _pUi->encoder1->setText(QString::number(_model.getMotorEncoder(1)));

    _model.setMotorSpeed(0, _pUi->motor0Slider->value());
    _model.setMotorSpeed(1, _pUi->motor1Slider->value());

    _pUi->speed0set->display( _model.getSettingMotorSpeed(0) );
    _pUi->speed1set->display( _model.getSettingMotorSpeed(1) );

    if( !_model.writeOutputs() )
    {
        _pErrorInfo->setText("Output write error");
    }
    else
    {
        _pErrorInfo->setText("");
    }
}

//-----------------------------------------------------------------------------
void MainWindow::on_dioconf0_currentIndexChanged(int index)
//-----------------------------------------------------------------------------
{
    _model.setConfigDioMode(0, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf1_currentIndexChanged(int index)
{
    _model.setConfigDioMode(1, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf2_currentIndexChanged(int index)
{
    _model.setConfigDioMode(2, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf3_currentIndexChanged(int index)
{
    _model.setConfigDioMode(3, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf4_currentIndexChanged(int index)
{
    _model.setConfigDioMode(4, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf5_currentIndexChanged(int index)
{
    _model.setConfigDioMode(5, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf6_currentIndexChanged(int index)
{
    _model.setConfigDioMode(6, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf7_currentIndexChanged(int index)
{
    _model.setConfigDioMode(7, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf8_currentIndexChanged(int index)
{
    _model.setConfigDioMode(8, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf9_currentIndexChanged(int index)
{
    _model.setConfigDioMode(9, (Ugv1::IoBoardModel::DioMode)index);
}
void MainWindow::on_dioconf10_currentIndexChanged(int index)
{
    _model.setConfigDioMode(10, (Ugv1::IoBoardModel::DioMode)index);
}

//-----------------------------------------------------------------------------
void MainWindow::on_encoderPpr_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigEncoderPPR(arg1);
}

//-----------------------------------------------------------------------------
void MainWindow::on_gearRatio_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigMotorGearRatio(arg1);
}

//-----------------------------------------------------------------------------
void MainWindow::on_wheelPerim_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigWheelPerimeter(arg1);
}

//-----------------------------------------------------------------------------
void MainWindow::on_modeconf_currentIndexChanged(int index)
//-----------------------------------------------------------------------------
{
    _model.setConfigMotorDriveMode((Ugv1::IoBoardModel::DriveControlMode)index);
}

//-----------------------------------------------------------------------------
void MainWindow::on_gainP_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigPGain(arg1&0xFF);
}

//-----------------------------------------------------------------------------
void MainWindow::on_gainI_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigIGain(arg1&0xFF);
}

//-----------------------------------------------------------------------------
void MainWindow::on_gainD_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setConfigDGain(arg1&0xFF);
}

//-----------------------------------------------------------------------------
void MainWindow::on_s0_valueChanged(int arg1)
//-----------------------------------------------------------------------------
{
    _model.setServoOut(0, arg1&0xff);
}
void MainWindow::on_s1_valueChanged(int arg1)
{
    _model.setServoOut(1, arg1&0xff);
}
void MainWindow::on_s2_valueChanged(int arg1)
{
    _model.setServoOut(2, arg1&0xff);
}
void MainWindow::on_s3_valueChanged(int arg1)
{
    _model.setServoOut(3, arg1&0xff);
}
void MainWindow::on_s4_valueChanged(int arg1)
{
    _model.setServoOut(4, arg1&0xff);
}
void MainWindow::on_s5_valueChanged(int arg1)
{
    _model.setServoOut(5, arg1&0xff);
}
void MainWindow::on_s6_valueChanged(int arg1)
{
    _model.setServoOut(6, arg1&0xff);
}
void MainWindow::on_s7_valueChanged(int arg1)
{
    _model.setServoOut(7, arg1&0xff);
}

//-----------------------------------------------------------------------------
void MainWindow::on_do0_toggled(bool checked)
//-----------------------------------------------------------------------------
{
    _model.setDigitalOut(0, checked);
}
void MainWindow::on_do1_toggled(bool checked)
{
    _model.setDigitalOut(1, checked);
}
void MainWindow::on_do2_toggled(bool checked)
{
    _model.setDigitalOut(2, checked);
}
void MainWindow::on_do3_toggled(bool checked)
{
    _model.setDigitalOut(3, checked);
}
void MainWindow::on_do4_toggled(bool checked)
{
    _model.setDigitalOut(4, checked);
}
void MainWindow::on_do5_toggled(bool checked)
{
    _model.setDigitalOut(5, checked);
}
void MainWindow::on_do6_toggled(bool checked)
{
    _model.setDigitalOut(6, checked);
}
void MainWindow::on_do7_toggled(bool checked)
{
    _model.setDigitalOut(7, checked);
}
void MainWindow::on_do8_toggled(bool checked)
{
    _model.setDigitalOut(8, checked);
}
void MainWindow::on_do9_toggled(bool checked)
{
    _model.setDigitalOut(9, checked);
}
void MainWindow::on_do10_toggled(bool checked)
{
    _model.setDigitalOut(10, checked);
}
