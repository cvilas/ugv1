# Common settings for UGV1 project libraries

GRAPE_DIR = ../../grape
LCM_DIR = C:\GitHub\ugv1\ThirdParty\lcm

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

INCLUDEPATH += $$PWD/../ $${GRAPE_DIR} $${LCM_DIR}/include
DEPENDPATH += $$PWD/../ $${GRAPE_DIR} $${LCM_DIR}/include

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
    else:unix: LIBS += -lUgv1Controllerd -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd -lUgv1Agentd -llcm -lpthread -lrt
} else {
    win32:LIBS += -lws2_32 -lUser32 -lUgv1Controller0 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0 -lUgv1Agent0
    else:unix: LIBS += -lUgv1Controller -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils -lUgv1Agent -llcm -lpthread -lrt
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

INCLUDEPATH += $${PWD} \
                $${PWD}/Controller \
                $${PWD}/Agent \
                $${GRAPE_DIR} \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io \
                $${LCM_DIR}/include

DEPENDPATH += ./

LIBS += -L$${PWD}/lib/ -L$${GRAPE_DIR}/lib -L$${LCM_DIR}/lib

