TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += IoBoard \
    tests/IoBoardTests

IoBoardTests.depends = IoBoard

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg 
