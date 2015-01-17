#-------------------------------------------------
#
# Project created by QtCreator 2014-01-14T14:11:56
#
#-------------------------------------------------

QT       += core\
            gui\
            xml

#QT       -= gui



TARGET = DataStructures
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

HEADERS += \
    manipulations.h \
    test.h \
    orthotic.h\
    xygrid.h \
    scan.h \
    basicstructures.h \
    patientdata.h \
    user.h \
    usermanager.h \
    patientmanager.h \
    scanmanager.h \
    orthoticmanager.h

SOURCES += main.cpp \
    manipulations.cpp \
    test.cpp \
    orthotic.cpp\
    xygrid.cpp \
    scan.cpp \
    basicstructures.cpp \
    patientdata.cpp \
    user.cpp \
    usermanager.cpp \
    patientmanager.cpp \
    scanmanager.cpp \
    orthoticmanager.cpp



include(common.pro)
include(shared.pro)

INCLUDEPATH += ../

INCLUDEPATH += ../shared/math\
               ../shared/ 

			   

