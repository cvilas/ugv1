# Common settings for UGV1 project libraries

GRAPE_DIR = ../../Grape

TEMPLATE = lib

# the following are used by individual libraries
VERSTR = '\\"$${VERSION}\\"'  # place quotes around the version string
DEFINES += VER=\"$${VERSTR}\" # create a VER macro containing the version string

# config settings
CONFIG += debug_and_release build_all resources thread
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
win32: DEFINES += GRAPE_DLL UGV1_DLL UGV1_DLL_EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIod0
    else:unix: LIBS += -lGrapeIod
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIo0
    else:unix: LIBS += -lGrapeIo
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=
INCLUDEPATH += $${PWD} \
                $${PWD}/common \
                $${GRAPE_DIR}/core \
                $${GRAPE_DIR}/io

DEPENDPATH += ./

LIBS += -L$${PWD}/lib/ -L$${GRAPE_DIR}/lib

