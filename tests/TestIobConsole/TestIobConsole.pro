#-------------------------------------------------
#
# Project created by QtCreator 2013-04-06T15:26:18
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = TestConsole
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

GRAPE_DIR = ../../../grape

DESTDIR = $${PWD}/../../bin
DLLDESTDIR = $${PWD}/../../bin/


SOURCES += main.cpp

INCLUDEPATH += $$PWD/../../IoBoard $$PWD/../../common $${GRAPE_DIR}
DEPENDPATH += $$PWD/../../IoBoard $$PWD/../../common $${GRAPE_DIR}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard0 -lGrapeCore0 -lGrapeIo0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoardd0 -lGrapeCored0 -lGrapeIod0
else:unix: LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard -lGrapeCore -lGrapeIo
