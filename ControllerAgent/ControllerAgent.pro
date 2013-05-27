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
    win32:LIBS += -lUgv1Agentd0
    else:unix: LIBS += -lUgv1Agentd
} else {
    win32:LIBS += -lUgv1Agent0
    else:unix: LIBS += -lUgv1Agent
}

HEADERS += \
    ControllerAgent.h
SOURCES += \
    ControllerAgent.cpp

lcmgen.target = ControllerMessage
lcmgen.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD/../ -x $$PWD/JoyMessage.lcm
QMAKE_EXTRA_TARGETS += lcmgen
PRE_TARGETDEPS = $$lcmgen.target

OTHER_FILES += \
    JoyMessage.lcm
