//==============================================================================
// Project  : UGV1
// Module   : ControllerApp
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "SerialPortDlg.h"
#include "ui_MainWindow.h"

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    _controller(_port)
{
    ui->setupUi(this);
}

//-----------------------------------------------------------------------------
MainWindow::~MainWindow()
//-----------------------------------------------------------------------------
{
    _controller.teardown();
    delete ui;
}

//-----------------------------------------------------------------------------
void MainWindow::onConnectSerialPort()
//-----------------------------------------------------------------------------
{
    _controller.teardown();

    QDialog* pDlg = new SerialPortDlg(_port, this);
    pDlg->show();
    pDlg->exec();

    if( _port.isOpen() )
    {
        _controller.setup();
    }
}
