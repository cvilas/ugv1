include(AgentExamples.pri)

TARGET = MessagePublisher

HEADERS +=
SOURCES += MessagePublisher.cpp

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}
