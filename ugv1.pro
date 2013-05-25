TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += Agent Controller HMI tests AgentRunner AgentExamples ControllerApp

tests.depends = Controller
HMI.depends = Controller
ControllerApp.depends = Controller

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist
