# get versions from this file
include(../version)

# library description
TARGET = IoBoard
VERSION = $${IO_BOARD_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./../common
DEPENDPATH += 

HEADERS += IoBoard.h \ 
    IoBoardCommand.h \
    IoBoardReply.h \
    IoBoardMessage.h
SOURCES += IoBoard.cpp \
    IoBoardCommand.cpp \
    IoBoardReply.cpp \
    IoBoardMessage.cpp
win32:SOURCES += 
unix:SOURCES += 
win32:LIBS += 
