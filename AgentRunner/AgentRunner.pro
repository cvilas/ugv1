# Common settings for Puce project executables

include(../ugvapps.pri)

TARGET = AgentRunner

win32: DEFINES += UGV1AGENT_DLL
CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
    win32:LIBS += -lUgv1Agentd0
    else:unix:LIBS += -lUgv1Agentd
} else {
    TARGET = $$join(TARGET,,,)
    win32:LIBS += -lUgv1Agent0
    else:unix:LIBS += -lUgv1Agent
}

SOURCES += \
    AgentRunner.cpp

