//==============================================================================
// Project  : UGV1
// Module   : TestIobModel
// File     : MainWindow.h
// Brief    : Main application window
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Vehicle/IoBoardModel.h>
#include <io/SerialPort.h>
#include <QTimer>

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
    void initialiseUi();

private slots:
    void onConnectSerialPort();
    void onTimer();
    void on_configBtn_clicked();
    void on_startStopBtn_clicked();
    void on_dioconf0_currentIndexChanged(int index);
    void on_dioconf1_currentIndexChanged(int index);
    void on_dioconf2_currentIndexChanged(int index);
    void on_dioconf3_currentIndexChanged(int index);
    void on_dioconf4_currentIndexChanged(int index);
    void on_dioconf5_currentIndexChanged(int index);
    void on_dioconf6_currentIndexChanged(int index);
    void on_dioconf7_currentIndexChanged(int index);
    void on_dioconf8_currentIndexChanged(int index);
    void on_dioconf9_currentIndexChanged(int index);
    void on_dioconf10_currentIndexChanged(int index);
    void on_encoderPpr_valueChanged(int arg1);
    void on_gearRatio_valueChanged(int arg1);
    void on_wheelPerim_valueChanged(int arg1);
    void on_modeconf_currentIndexChanged(int index);
    void on_gainP_valueChanged(int arg1);
    void on_gainI_valueChanged(int arg1);
    void on_gainD_valueChanged(int arg1);
    void on_s0_valueChanged(int arg1);
    void on_s1_valueChanged(int arg1);
    void on_s2_valueChanged(int arg1);
    void on_s3_valueChanged(int arg1);
    void on_s4_valueChanged(int arg1);
    void on_s5_valueChanged(int arg1);
    void on_s6_valueChanged(int arg1);
    void on_s7_valueChanged(int arg1);
    void on_do0_toggled(bool checked);
    void on_do1_toggled(bool checked);
    void on_do2_toggled(bool checked);
    void on_do3_toggled(bool checked);
    void on_do4_toggled(bool checked);
    void on_do5_toggled(bool checked);
    void on_do6_toggled(bool checked);
    void on_do7_toggled(bool checked);
    void on_do8_toggled(bool checked);
    void on_do9_toggled(bool checked);
    void on_do10_toggled(bool checked);

private:
    void updateStatusBar();

private:
    Ui::MainWindow *_pUi;
    Grape::SerialPort _port;
    Ugv1::IoBoard _iob;
    Ugv1::IoBoardModel _model;
    QLabel* _pConnectionStatus;
    QLabel* _pErrorInfo;
    QTimer _timer;
};

#endif // MAINWINDOW_H
