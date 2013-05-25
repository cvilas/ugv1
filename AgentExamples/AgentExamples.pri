include(../ugvapps.pri)

lcmgen.target = example_t.hpp
lcmgen.commands = $${LCM_DIR}/bin/lcm-gen --cpp-hpath $$PWD -x $$PWD/example_t.lcm
QMAKE_EXTRA_TARGETS += lcmgen
PRE_TARGETDEPS = $$lcmgen.target

OTHER_FILES += \
    example_t.lcm

HEADERS += AgentExamples.h
