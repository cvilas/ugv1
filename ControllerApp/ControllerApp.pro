TARGET = ControllerApp
include(../ugvapps.pri)

SOURCES += main.cpp\
        MainWindow.cpp \
        SerialPortDlg.cpp

HEADERS  += MainWindow.h \
    SerialPortDlg.h

FORMS    += MainWindow.ui \
    SerialPortConnect.ui
