# Common settings for UGV1 project libraries

GRAPE_DIR = ../../grape
LCM_DIR = C:\GitHub\ugv1\ThirdParty\lcm

TEMPLATE = lib

# the following are used by individual libraries
VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string

# config settings
CONFIG += debug_and_release build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

CONFIG += qt
QT += core xml
QT -= gui

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

# target directories
DESTDIR = $${PWD}/lib
DLLDESTDIR = $${PWD}/bin/

DEFINES +=
win32: DEFINES += GRAPECORE_DLL GRAPEIO_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTimingd0 -lGrapeCored0 -lGrapeIod0 -lGrapeUtilsd0 -llcmd
    else:unix: LIBS += -lGrapeIod -lGrapeTimingd -lGrapeCored -lGrapeUtilsd -llcm -lpthread -lrt
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeTiming0 -lGrapeCore0 -lGrapeIo0 -lGrapeUtils0 -llcm
    else:unix: LIBS += -lGrapeIo -lGrapeTiming -lGrapeCore -lGrapeUtils -llcm -lpthread -lrt
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

