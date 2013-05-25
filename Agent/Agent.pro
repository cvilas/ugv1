# get versions from this file
include(../version)

# library description
TARGET = Ugv1Agent
VERSION = $${AGENTLIB_VERSION}

# common settings
include(../ugvlibs.pri)

# library specific settings
INCLUDEPATH += ./
DEPENDPATH +=

win32:DEFINES += UGV1AGENTLIB_DLL UGV1AGENTLIB_DLL_EXPORT

HEADERS += \
    AgentException.h \
    IAgent.h \
    AgentBus.h \
    agent_common.h \
    AgentMessenger.h
SOURCES += \
    IAgent.cpp \
    AgentBus.cpp \
    AgentMessenger.cpp
win32:SOURCES +=
unix:SOURCES +=
win32:LIBS +=
