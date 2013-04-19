TARGET = IoBoardTests

include(../ugvtests.pri)
QT       += testlib
QT       -= gui
CONFIG   += console
CONFIG   -= app_bundle

win32: DEFINES += GRAPE_DLL UGV1VEHICLELIB_DLL

SOURCES += IoBoard_tests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

