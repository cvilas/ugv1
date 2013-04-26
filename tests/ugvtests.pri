# Common settings for UGV1 project libraries

GRAPE_DIR = ../../../grape
QT       += core gui
CONFIG += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# config settings
CONFIG += \
    debug \
    #debug_and_release \
    build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

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
    win32:LIBS += -lws2_32 -lUser32 -lUgv1Controllerd0 -lGrapeCored0 -lGrapeIod0
    else:unix: LIBS += -lUgv1Controllerd -lGrapeIod -lGrapeCored
} else {
    win32:LIBS += -lws2_32 -lUser32 -lUgv1Controller0 -lGrapeCore0 -lGrapeIo0
    else:unix: LIBS += -lUgv1Controller -lGrapeIo -lGrapeCore
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

INCLUDEPATH += $${PWD} \
                $${PWD}/Controller \
                $${GRAPE_DIR} \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io

DEPENDPATH += ./

LIBS += -L$${PWD}/../lib/ -L$${GRAPE_DIR}/lib

