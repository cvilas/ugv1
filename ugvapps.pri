# Common settings for UGV1 project libraries

GRAPE_DIR = ../../grape
win32:THIRD_PARTY_LIBS = $${PWD}/ThirdParty/windows
unix:!android: THIRD_PARTY_LIBS = /usr/local
android: THIRD_PARTY_LIBS = $${PWD}/ThirdParty/android

QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

# config settings
CONFIG += \
    debug_and_release \
    build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

DESTDIR = $${PWD}/bin
DLLDESTDIR = $${PWD}/bin/

INCLUDEPATH += $${PWD} \
                $${PWD}/Controller \
                $${PWD}/Agent \
                $${GRAPE_DIR} \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io \
                $${THIRD_PARTY_LIBS}/include
DEPENDPATH += $$PWD/../ $${GRAPE_DIR} $${THIRD_PARTY_LIBS}/include
LIBS += -L$${PWD}/lib/ -L$${GRAPE_DIR}/lib -L$${THIRD_PARTY_LIBS}/lib

DEFINES +=
win32: DEFINES += GRAPECORE_DLL GRAPEIO_DLL UGV1AGENTLIB_DLL UGV1CONTROLLER_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lUgv1Controllerd0 -lGrapeTimingd0 -lGrapeCored0 -lGrapeIod0 -lGrapeUtilsd0 -lUgv1Agentd0
    else:unix: LIBS += -lUgv1Agentd -lUgv1Controllerd -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd -llcm
    unix:!android: LIBS += -lpthread -lrt
} else {
    win32:LIBS += -lUgv1Agent0 -lws2_32 -lUser32 -lUgv1Controller0 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0
    else:unix: LIBS += -lUgv1Agent -lUgv1Controller -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils -llcm
    unix:!android: LIBS += -lpthread -lrt
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

