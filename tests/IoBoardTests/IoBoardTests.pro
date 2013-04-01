
#-------------------------------------------------
#
# Project created by QtCreator 2013-04-01T21:12:22
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = IoBoardTests
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

win32: DEFINES += GRAPE_DLL UGV1_DLL

SOURCES += IoBoard_tests.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -lIoBoard0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -lIoBoardd0
else:unix: LIBS += -L$$PWD/../../lib/ -lIoBoard

INCLUDEPATH += $$PWD/../../IoBoard $$PWD/../../common
DEPENDPATH += $$PWD/../../IoBoard $$PWD/../../common
