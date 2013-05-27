# get versions from this file
include(../version)

# library description
TARGET = Ugv1ControllerAgent
VERSION = $${CONTROLLERAGENTLIB_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./ ../Ugv1Messages
DEPENDPATH +=

win32:DEFINES += UGV1AGENTLIB_DLL UGV1CONTROLLERAGENT_DLL UGV1CONTROLLERAGENT_DLL_EXPORT
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lUgv1Controllerd0 -lUgv1Agentd0
    else:unix: LIBS += -lUgv1Controllerd -lUgv1Agentd
} else {
    win32:LIBS += -lUgv1Controller0 -lUgv1Agent0
    else:unix: LIBS += -lUgv1Controller -lUgv1Agent
}

HEADERS += \
    ControllerAgent.h
SOURCES += \
    ControllerAgent.cpp

lcm_odomsg.target = OdometryMessage
lcm_odomsg.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/OdometryMessage.lcm
QMAKE_EXTRA_TARGETS += lcm_odomsg
PRE_TARGETDEPS += $$lcm_odomsg.target

lcm_cmdmsg.target = CommandMessage
lcm_cmdmsg.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/CommandMessage.lcm
QMAKE_EXTRA_TARGETS += lcm_cmdmsg
PRE_TARGETDEPS += $$lcm_cmdmsg.target

OTHER_FILES += \
    OdometryMessage.lcm \
    CommandMessage.lcm
