TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += Agent Controller HMI tests AgentRunner JoystickAgent ControllerAgent AgentExamples

tests.depends = Controller
HMI.depends = Controller
ControllerAgent.depends = Controller

OTHER_FILES += \
    ugvapps.pri \
    ugvlibs.pri \
    doc/doxygen/doxydoc.h \
    doc/doxygen/doxygen.cfg \
    etc/tasklist \
    config/ugv1_linux.cfg \
    config/ugv1_windows.cfg 
