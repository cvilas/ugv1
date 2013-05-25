include(../ugvapps.pri)

lcmgen.target = example_t.hpp
win32:lcmgen.commands = ../ThirdParty/lcm-w32-0-9-0/bin/lcm-gen.exe -x example_t.lcm
else:unix:lcmgen.commands = lcm-gen -x $$PWD/example_t.lcm --cpp-hpath $$PWD
QMAKE_EXTRA_TARGETS += lcmgen
PRE_TARGETDEPS = $$lcmgen.target

OTHER_FILES += \
    example_t.lcm

HEADERS += AgentExamples.h
