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

QMAKE_EXTRA_TARGETS += lcm_odomsg
!android:PRE_TARGETDEPS += lcm_odomsg
lcm_odomsg.commands += $${THIRD_PARTY_LIBS}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/OdometryMessage.lcm

QMAKE_EXTRA_TARGETS += lcm_cmdmsg
!android:PRE_TARGETDEPS += lcm_cmdmsg
lcm_cmdmsg.commands += $${THIRD_PARTY_LIBS}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/CommandMessage.lcm

QMAKE_EXTRA_TARGETS += lcm_healthmsg
!android:PRE_TARGETDEPS += lcm_healthmsg
lcm_healthmsg.commands += $${THIRD_PARTY_LIBS}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/HealthMessage.lcm


OTHER_FILES += \
    OdometryMessage.lcm \
    CommandMessage.lcm \
    HealthMessage.lcm
