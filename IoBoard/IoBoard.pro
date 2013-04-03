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
    IoBoardCommand_servo.h \
    IoBoardCommand_dio.h \
    IoBoardCommand_analog.h \
    IoBoardCommand_rs485.h \
    IoBoardCommand_i2c.h \
    IoBoardCommand_motor.h \
    IoBoardReply.h \
    IoBoardMessage.h
SOURCES += IoBoard.cpp \
    IoBoardCommand.cpp \
    IoBoardReply.cpp \
    IoBoardMessage.cpp
win32:SOURCES += 
unix:SOURCES += 
win32:LIBS += 
