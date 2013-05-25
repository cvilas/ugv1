# get versions from this file
include(../version)

# library description
TARGET = Ugv1JoystickAgent
VERSION = $${JOYSTICKAGENTLIB_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./ ../Ugv1Messages
DEPENDPATH +=

win32:DEFINES += UGV1JOYSTICKAGENT_DLL UGV1JOYSTICKAGENT_DLL_EXPORT

HEADERS += \
    JoystickAgent.h
SOURCES += \
    JoystickAgent.cpp

lcmgen.target = JoyMessage.hpp
lcmgen.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/JoyMessage.lcm
QMAKE_EXTRA_TARGETS += lcmgen
PRE_TARGETDEPS = $$lcmgen.target

OTHER_FILES += \
    JoyMessage.lcm
