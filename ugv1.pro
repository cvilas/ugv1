TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += IoBoard \
    Controller \
    tests/IoBoardTests \
    tests/TestGui \
    tests/TestConsole \

IoBoardTests.depends = IoBoard

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist
