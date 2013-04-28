TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += Controller HMI tests \
    ControllerApp

tests.depends = Controller
HMI.depends = Controller
ControllerApp.depends = Controller

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist
