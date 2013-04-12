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

win32:DEFINES += UGV1IOB_DLL UGV1IOB_DLL_EXPORT

HEADERS += IoBoard.h \ 
    IoBoardMessage.h \
    IoBoardMessage_analog.h \
    IoBoardMessage_dio.h \
    IoBoardMesssage_i2c.h \
    IoBoardMessage_motor.h \
    IoBoardMessage_rs485.h \
    IoBoardMessage_servo.h \
    IoBoardModel.h \
    ioboard_common.h
SOURCES += IoBoard.cpp \
    IoBoardMessage.cpp \
    IoBoardMessage_analog.cpp \
    IoBoardMessage_dio.cpp \
    IoBoardMessage_motor.cpp \
    IoBoardMessage_servo.cpp \
    IoBoardModel.cpp
win32:SOURCES += 
unix:SOURCES += 
win32:LIBS += 
