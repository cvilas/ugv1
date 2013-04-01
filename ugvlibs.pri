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
DESTDIR = ../lib
DLLDESTDIR = ../bin/

DEFINES +=
win32: DEFINES += GRAPE_DLL UGV_DLL UGV_DLL_EXPORT _UNICODE _CRT_SECURE_NO_WARNINGS
CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIod
    else:unix: LIBS += -lGrapeIod
} else {
    win32:LIBS += -lws2_32 -lUser32 -lGrapeIo
    else:unix: LIBS += -lGrapeIo
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=
INCLUDEPATH += ./
win32:INCLUDEPATH += $${GRAPE_DIR} \
                     ../ThirdParty/pthreads-w32-2-8-0/include


DEPENDPATH += ./

LIBS += -L../lib/ #-llcm
win32:LIBS += -L../Grape/lib \
              -L../ThirdParty/pthreads-w32-2-8-0/lib

