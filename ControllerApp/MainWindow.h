//==============================================================================
// Project  : UGV1
// Module   : ControllerApp
// File     : MainWindow.h
// Brief    : Main application window
//==============================================================================

#ifndef UGV1_MAINWINDOW_H
#define UGV1_MAINWINDOW_H

#include <QMainWindow>
#include "Controller/Controller.h"
#include "io/SerialPort.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void onConnectSerialPort();

private:
    Ui::MainWindow *ui;
    Grape::SerialPort _port;
    Ugv1::Controller _controller;
};

#endif // UGV1_MAINWINDOW_H
