include(AgentExamples.pri)

TARGET = JoystickSubscriber2

HEADERS +=
SOURCES += JoystickSubscriber2.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
