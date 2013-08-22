#-------------------------------------------------
#
# Project created by QtCreator 2013-07-18T15:20:02
#
#-------------------------------------------------

GRAPE_DIR = /home/vilas/Projects/mine/grape
UGV1_DIR = /home/vilas/Projects/mine/ugv1

win32:THIRD_PARTY_LIBS = $${UGV1_DIR}/ThirdParty/windows
unix:!android: THIRD_PARTY_LIBS = /usr/local
android: THIRD_PARTY_LIBS = $${UGV1_DIR}/ThirdParty/android

QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Publisher
TEMPLATE = app

# config settings
CONFIG += \
    debug_and_release \
    build_all resources thread
android:CONFIG += static
win32: CONFIG += dll embed_manifest_dll embed_manifest_exe

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

#DESTDIR = $${PWD}/bin
#DLLDESTDIR = $${PWD}/bin/

INCLUDEPATH += $${PWD} \
                $${GRAPE_DIR} \
                $${UGV1_DIR} \
                $${THIRD_PARTY_LIBS}/include

DEPENDPATH += $$PWD/../ $${GRAPE_DIR} $${THIRD_PARTY_LIBS}/include
LIBS += -L$${PWD}/lib/ -L$${UGV1_DIR}/lib -L$${GRAPE_DIR}/lib -L$${THIRD_PARTY_LIBS}/lib

DEFINES +=
win32: DEFINES += GRAPECORE_DLL UGV1AGENTLIB_DLL UNICODE _UNICODE _CRT_SECURE_NO_WARNINGS

build_pass:CONFIG(debug, release|debug) {
    TARGET = $$join(TARGET,,,d)
} else {
    TARGET = $$join(TARGET,,,)
}

CONFIG(debug, release|debug) {
    DEFINES += _DEBUG
    win32:LIBS += -lws2_32 -lUser32 -lGrapeCored0 -lUgv1Agentd0
    else:unix: LIBS += -lUgv1Agentd -lGrapeCored -llcm
    unix:!android: LIBS += -lpthread -lrt
} else {
    win32:LIBS += -lUgv1Agent0 -lws2_32 -lUser32 -lGrapeCore0
    else:unix: LIBS += -lUgv1Agent -lGrapeCore -llcm
    unix:!android: LIBS += -lpthread -lrt
}

# don't want linking against qtmain.lib
QMAKE_LIBS_QT_ENTRY=

