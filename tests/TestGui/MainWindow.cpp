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
    if( !_iob.getVersion(response) )
    {
        int code;
        _pErrorInfo->setText(QString::fromStdString(_iob.getLastError(code)));
    }
    else
    {
        _pErrorInfo->setText("");
    }

    if( response.isValid() )
    {
        QString vinfo = QString::number(response.getBoardCode()) + QString(":")
                + QString::number(response.getBoardVersion()) + QString(":")
                + QString::number(response.getBoardRevision());
        _pUi->versionTxt->setText(vinfo);
    }
    else
    {
        _pUi->versionTxt->setText("error");
    }

}

//-----------------------------------------------------------------------------
void MainWindow::on_getAnalogBtn_clicked()
//-----------------------------------------------------------------------------
{
    Ugv1::ReadAnalogInResponse response;
    if( !_iob.getAnalog(response) )
    {
        int code;
        _pErrorInfo->setText(QString::fromStdString(_iob.getLastError(code)));
    }
    else
    {
        _pErrorInfo->setText("");
    }

    if( response.isValid() )
    {
        _pUi->analog0->setText(QString::number(response.getVolts(0)));
        _pUi->analog1->setText(QString::number(response.getVolts(1)));
        _pUi->analog2->setText(QString::number(response.getVolts(2)));
        _pUi->analog3->setText(QString::number(response.getVolts(3)));
        _pUi->analog4->setText(QString::number(response.getVolts(4)));
        _pUi->analog5->setText(QString::number(response.getVolts(5)));
        _pUi->analog6->setText(QString::number(response.getVolts(6)));
        _pUi->analog7->setText(QString::number(response.getVolts(7)));
    }
}

