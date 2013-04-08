# get versions from this file
include(../version)

# library description
TARGET = Controller
VERSION = $${CONTROLLER_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./../common
DEPENDPATH +=

HEADERS += Controller.h \
    Controller.h
SOURCES += Controller.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=



