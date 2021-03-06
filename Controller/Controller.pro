# get versions from this file
include(../version)

# library description
TARGET = Ugv1Controller
VERSION = $${CONTROLLER_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./../common
DEPENDPATH += 

win32:DEFINES += UGV1CONTROLLER_DLL UGV1CONTROLLER_DLL_EXPORT

HEADERS += IoBoard.h \ 
    IoBoardMessage.h \
    IoBoardMessage_analog.h \
    IoBoardMessage_dio.h \
    IoBoardMessage_i2c.h \
    IoBoardMessage_motor.h \
    IoBoardMessage_rs485.h \
    IoBoardMessage_servo.h \
    IoBoardModel.h \
    RobotModel.h \
	ControllerException.h \
    controller_common.h
SOURCES += IoBoard.cpp \
    IoBoardMessage.cpp \
    IoBoardMessage_analog.cpp \
    IoBoardMessage_dio.cpp \
    IoBoardMessage_motor.cpp \
    IoBoardMessage_servo.cpp \
    IoBoardModel.cpp \
    RobotModel.cpp \
	ControllerException.cpp
win32:SOURCES += 
unix:SOURCES += 
win32:LIBS += 
