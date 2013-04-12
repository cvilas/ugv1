//==============================================================================
// Project  : UGV1
// Module   : TestGui
// File     : MainWindow.h
// Brief    : Main application window
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <io/SerialPort.h>
#include <IoBoard.h>

namespace Ui {
class MainWindow;
}

class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onConnectSerialPort();
    void on_getAnalogBtn_clicked();
    void on_setTimeoutBtn_clicked();
    void on_getVersionBtn_clicked();
    void on_dioConfigBtn_clicked();
    void on_doSetBtn_clicked();
    void on_configServoBtn_clicked();
    void on_setServoBtn_clicked();
    void on_getDinBtn_clicked();
    void on_setDrvTrainBtn_clicked();
    void on_setPidBtn_clicked();
    void on_resetEncBtn_clicked();
    void on_getEncBtn_clicked();
    void on_pidControlOn_toggled(bool checked);
    void on_motor1Slider_sliderMoved(int position);
    void on_motor2Slider_sliderMoved(int position);
    void on_getCurrentBtn_clicked();
    void on_getSpeedBtn_clicked();

private:
    void updateStatusBar();

private:
    Ui::MainWindow *_pUi;
    Grape::SerialPort _port;
    Ugv1::IoBoard _iob;

    QLabel* _pConnectionStatus;
    QLabel* _pErrorInfo;

    Ugv1::WriteMotorSpeedCommand _motorSpeedCmd;
    Ugv1::WriteMotorPowerCommand _motorPowerCmd;
};

#endif // MAINWINDOW_H
