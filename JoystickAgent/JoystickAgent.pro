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

win32:DEFINES += UGV1AGENTLIB_DLL UGV1JOYSTICKAGENT_DLL UGV1JOYSTICKAGENT_DLL_EXPORT
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lUgv1Agentd0
    else:unix: LIBS += -lUgv1Agentd
} else {
    win32:LIBS += -lUgv1Agent0
    else:unix: LIBS += -lUgv1Agent
}

HEADERS += \
    JoystickAgent.h
SOURCES += \
    JoystickAgent.cpp

QMAKE_EXTRA_TARGETS += lcm_jsmsg
lcm_jsmsg.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/JoyMessage.lcm
!android:PRE_TARGETDEPS += lcm_jsmsg

OTHER_FILES += \
    JoyMessage.lcm
