#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T09:19:32
#
#-------------------------------------------------

GRAPE_DIR = ../../../grape
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestGui
TEMPLATE = app

DESTDIR = $${PWD}/../../lib
DLLDESTDIR = $${PWD}/../../bin/

SOURCES += main.cpp\
        MainWindow.cpp \
    SerialPortDlg.cpp

HEADERS  += MainWindow.h \
    SerialPortDlg.h

FORMS    += MainWindow.ui \
    SerialPortConnect.ui

INCLUDEPATH += $$PWD/../../IoBoard $$PWD/../../common $${GRAPE_DIR}
DEPENDPATH += $$PWD/../../IoBoard $$PWD/../../common $${GRAPE_DIR}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard0 -lGrapeCore0 -lGrapeIo0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoardd0 -lGrapeCored0 -lGrapeIod0
else:unix: LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard -lGrapeCore -lGrapeIo