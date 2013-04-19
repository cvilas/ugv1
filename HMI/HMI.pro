# get versions from this file
include(../version)

# library description
TARGET = HMI
VERSION = $${HMI_VERSION}

# common settings
include(../ugvlibs.pri)
QT       += network

# library specific settings
INCLUDEPATH += ./../common
DEPENDPATH +=

win32:DEFINES += UGV1HMI_DLL UGV1HMI_DLL_EXPORT

HEADERS += HMIMessageServer.h \
	HMIMessageClient.h \
    hmi_common.h
SOURCES += HMIMessageServer.cpp HMIMessageClient.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=
