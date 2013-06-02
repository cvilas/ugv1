# Common settings for UGV1 project libraries

GRAPE_DIR = ../../../grape
QT       += core gui
CONFIG += console
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

QT       += testlib
CONFIG   += console
CONFIG   -= app_bundle

android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

DESTDIR = $${PWD}/../bin
DLLDESTDIR = $${PWD}/../bin/

INCLUDEPATH += $$PWD/../ $${GRAPE_DIR}
DEPENDPATH += $$PWD/../ $${GRAPE_DIR}

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS +=  -lUgv1Controllerd0 -lGrapeIod0 -lGrapeTimingd0 -lGrapeCored0 -lws2_32 -lUser32
    else:unix: LIBS += -lUgv1Controllerd -lGrapeIod -lGrapeTimingd -lGrapeCored -lpthread -lrt
} else {
    win32:LIBS += -lUgv1Controller0 -lGrapeIo0 -lGrapeTiming0 -lGrapeCore0 -lws2_32 -lUser32
    else:unix: LIBS += -lUgv1Controller -lGrapeIo -lGrapeTiming -lGrapeCore -lpthread -lrt
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

