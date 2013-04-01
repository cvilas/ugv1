# get versions from this file
include(../version)

# library description
TARGET = IoBoard
VERSION = $${IO_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
win32:DEFINES -= _UNICODE UNICODE
INCLUDEPATH += ../common 
DEPENDPATH += 

HEADERS += IoBoard.h \ 
    IoBoardCommand.h \
    IoBoardReply.h
SOURCES += IoBoard.cpp \
    IoBoardCommand.cpp \
    IoBoardReply.cpp
win32:SOURCES += 
unix:SOURCES += 
win32:LIBS += 
