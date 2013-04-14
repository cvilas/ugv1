//==============================================================================
// Project  : UGV1
// Module   : TestGui
// File     : SerialPortDlg.cpp
//==============================================================================

#include "SerialPortDlg.h"
#include "ui_SerialPortConnect.h"
#ifdef __ANDROID__
#include <QProcess>
#endif

//==============================================================================
SerialPortDlg::SerialPortDlg(Grape::SerialPort& port, QWidget *parent)
//==============================================================================
    : QDialog(parent), pUi(new Ui::SerialPortConnectUI), _port(port)
{
    pUi->setupUi(this);

    _baud = (Grape::SerialPort::BaudRate)(pUi->baudSel->currentIndex());
    _port.setPortName(pUi->portSel->text().toStdString());

    updateStatus();
}

//-----------------------------------------------------------------------------
SerialPortDlg::~SerialPortDlg()
//-----------------------------------------------------------------------------
{
    delete pUi;
}

//-----------------------------------------------------------------------------
void SerialPortDlg::updateStatus()
//-----------------------------------------------------------------------------
{
    if( _port.isOpen() )
    {
        pUi->portSel->setText(QString::fromStdString(_port.getPortName()));
        pUi->portSel->setDisabled(true);
        pUi->connectBtn->setText("Disconnect");
        pUi->statusLbl->setText("connected");
    }
    else
    {
        pUi->portSel->setDisabled(false);
        pUi->connectBtn->setText("Connect");
        pUi->statusLbl->setText("disconnected");
    }
}

//-----------------------------------------------------------------------------
void SerialPortDlg::on_baudSel_activated(int index)
//-----------------------------------------------------------------------------
{
    _baud = (Grape::SerialPort::BaudRate)index;
    if( _port.isOpen() )
    {
        _port.setBaudRate(_baud);
    }
}

//-----------------------------------------------------------------------------
void SerialPortDlg::on_portSel_editingFinished()
//-----------------------------------------------------------------------------
{
    _port.setPortName(pUi->portSel->text().toStdString());
}

//-----------------------------------------------------------------------------
void SerialPortDlg::on_connectBtn_clicked()
//-----------------------------------------------------------------------------
{
    if( _port.isOpen() )
    {
        _port.close();
    }
    else
    {
#ifdef __ANDROID__
        //QProcess p;
        //QString cmd = QString("su -c 'chmod 666 ") + pUi->portSel->text() + QString("'");
        //p.start(cmd);
        //p.waitForFinished(-1);
#endif
        _port.open();
        if( _port.isOpen() )
        {
            _port.setBaudRate(_baud);
            _port.setDataFormat(Grape::SerialPort::D8N1);
        }
    }
    updateStatus();
}
