include(../ugvapps.pri)

TARGET = AgentRunner
CONFIG += console
win32: DEFINES += UGV1AGENT_DLL
CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

SOURCES += \
    AgentRunner.cpp

