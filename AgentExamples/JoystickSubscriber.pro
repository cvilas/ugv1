include(AgentExamples.pri)

TARGET = JoystickSubscriber

HEADERS +=
SOURCES += JoystickSubscriber.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
