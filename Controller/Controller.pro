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

win32:DEFINES += UGV1CONTROLLER_DLL UGV1CONTROLLER_DLL_API UGV1IOB_DLL

HEADERS += Controller.h \
    Controller.h \
    controller_common.h
SOURCES += Controller.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lIoBoardd0
    else:unix: LIBS += -lIoBoardd
} else {
    win32:LIBS += -lIoBoard0
    else:unix: LIBS += -lIoBoard
}


