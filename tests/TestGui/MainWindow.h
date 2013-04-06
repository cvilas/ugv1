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
private:
    void updateStatusBar();
private:
    Ui::MainWindow *_pUi;
    Grape::SerialPort _port;
    Ugv1::IoBoard _iob;

    QLabel* _pConnectionStatus;
    QLabel* _pErrorInfo;
};

#endif // MAINWINDOW_H
