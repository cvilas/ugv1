TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += IoBoard \
    tests/IoBoardTests

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg 