QT       += core\
            gui\
            xml

TARGET = CoreSystem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp 


INCLUDEPATH += ../
        

include(shared-math.pro)
include(shared-stl.pro)
include(shared-amf.pro)
#include(common.pro)
include(shared.pro)
include(DataStructures.pro)
include(Controllers.pro)

