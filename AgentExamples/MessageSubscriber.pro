include(AgentExamples.pri)

TARGET = MessageSubscriber

HEADERS +=
SOURCES += MessageSubscriber.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
