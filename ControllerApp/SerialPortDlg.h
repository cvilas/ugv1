//==============================================================================
// Project  : UGV1
// Module   : Gui
// File     : SerialPortDlg.h
// Brief    : Serial port settings configuration
//==============================================================================

#ifndef SERIALPORTDLG_H
#define SERIALPORTDLG_H

#include <QDialog>
#include <io/SerialPort.h>

namespace Ui {
class SerialPortConnectUI;
}

class SerialPortDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPortDlg(Grape::SerialPort& port, QWidget *parent = 0);
    ~SerialPortDlg();

private slots:
    void on_baudSel_activated(int index);
    void on_portSel_editingFinished();
    void on_connectBtn_clicked();
private:
    void updateStatus();

private:
    Ui::SerialPortConnectUI *pUi;
    Grape::SerialPort& _port;
    Grape::SerialPort::BaudRate _baud;
};

#endif // SERIALPORTDLG_H
