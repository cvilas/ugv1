
#-------------------------------------------------
#
# Project created by QtCreator 2013-04-01T21:12:22
#
#-------------------------------------------------

QT       += testlib
QT       -= gui

GRAPE_DIR = ../../../grape

TARGET = IoBoardTests

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DESTDIR = $${PWD}/../../lib
DLLDESTDIR = $${PWD}/../../bin/

win32: DEFINES += GRAPE_DLL UGV1_DLL

SOURCES += IoBoard_tests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard0 -lGrapeCore0 -lGrapeIo0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoardd0 -lGrapeCored0 -lGrapeIod0
else:unix: LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lIoBoard -lGrapeCore -lGrapeIo

INCLUDEPATH += $$PWD/../../IoBoard $$PWD/../../common
DEPENDPATH += $$PWD/../../IoBoard $$PWD/../../common
