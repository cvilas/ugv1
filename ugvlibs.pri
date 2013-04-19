# Common settings for UGV1 project libraries

GRAPE_DIR = ../../grape

TEMPLATE = lib

# the following are used by individual libraries
VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string

# config settings
CONFIG += debug_and_release build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

CONFIG += qt
QT += core
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

