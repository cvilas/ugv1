TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += Agent Controller HMI tests AgentRunner JoystickAgent AgentExamples ControllerApp

tests.depends = Controller
HMI.depends = Controller
ControllerApp.depends = Controller

OTHER_FILES += \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist \
    config/ugv1_linux.cfg \
    config/ugv1_windows.cfg
