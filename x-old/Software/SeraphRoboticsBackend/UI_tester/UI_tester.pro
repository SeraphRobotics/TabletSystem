#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T07:02:21
#
#-------------------------------------------------

QT       += core\
            gui\
            xml

TARGET = UI_tester
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    UI_structs.cpp \
    mastercontrolunit.cpp \
    testing/test.cpp

HEADERS += \
    UI_structs.h \
    mastercontrolunit.h \
    testing/test.h

INCLUDEPATH += ../

include(shared-math.pri)
include(shared-stl.pri)
include(shared-amf.pri)
#include(common.pri)
include(shared.pri)
include(DataStructures.pri)

