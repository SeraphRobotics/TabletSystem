#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T07:02:21
#
#-------------------------------------------------

QT       += core\
            gui\
            xml

TARGET = CoreSystem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp 


HEADERS += \
    UI_structs.h \
    mastercontrolunit.h \
    testing/test.h

INCLUDEPATH += ../
        

include(shared-math.pro)
include(shared-stl.pro)
include(shared-amf.pro)
#include(common.pro)
include(shared.pro)
include(DataStructures.pro)

