TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += IoBoard \
    tests/IoBoardTests \
    tests/TestGui \
    tests/TestConsole

IoBoardTests.depends = IoBoard

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg 
