# Common settings for UGV1 project libraries

GRAPE_DIR = ../../../grape
QT       += core gui

TEMPLATE = app

DESTDIR = $${PWD}/../../bin
DLLDESTDIR = $${PWD}/../../bin/

INCLUDEPATH += $$PWD/../../ $$PWD/../../Vehicle $${GRAPE_DIR}
DEPENDPATH += $$PWD/../../ $$PWD/../../Vehicle $${GRAPE_DIR}

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lvehicle0 -lGrapeCore0 -lGrapeIo0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lvehicled0 -lGrapeCored0 -lGrapeIod0
else:unix: LIBS += -L$$PWD/../../lib/ -L$${GRAPE_DIR}/lib -lvehicle -lGrapeCore -lGrapeIo

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

DEFINES +=
win32: DEFINES += GRAPECORE_DLL GRAPEIO_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIod0 -lGrapeCored0
    else:unix: LIBS += -lGrapeIod -lGrapeCored
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIo0 -lGrapeCore0
    else:unix: LIBS += -lGrapeIo -lGrapeCore
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=
INCLUDEPATH += $${PWD} \
                $${PWD}/Vehicle \
                $${GRAPE_DIR} \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io

DEPENDPATH += ./

LIBS += -L$${PWD}/lib/ -L$${GRAPE_DIR}/lib

