# Common settings for UGV1 project libraries

GRAPE_DIR = ../../../grape
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin/

INCLUDEPATH += $$PWD/../ $${GRAPE_DIR}
DEPENDPATH += $$PWD/../ $${GRAPE_DIR}

DEFINES +=
win32: DEFINES += GRAPECORE_DLL GRAPEIO_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lVehicled0 -lGrapeCored0 -lGrapeIod0
    else:unix: LIBS += -lVehicled -lGrapeIod -lGrapeCored
} else {
    win32:LIBS += -lws2_32 -lUser32 -lVehicle0 -lGrapeCore0 -lGrapeIo0
    else:unix: LIBS += -lVehicle -lGrapeIo -lGrapeCore
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

INCLUDEPATH += $${PWD} \
                $${PWD}/Vehicle \
                $${GRAPE_DIR} \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io

DEPENDPATH += ./

LIBS += -L$${PWD}/../lib/ -L$${GRAPE_DIR}/lib

