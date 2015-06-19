#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T10:08:57
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Scanner
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

DEFINES += DEBUGGING

SOURCES += main.cpp \
    scanningmcu.cpp
INCLUDEPATH += ../../
INCLUDEPATH += ../
include("seraphlibs.pro")

HEADERS += \
    scanningmcu.h
