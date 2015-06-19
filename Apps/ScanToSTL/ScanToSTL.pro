#-------------------------------------------------
#
# Project created by QtCreator 2015-06-19T12:36:26
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ScanToSTL
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += ../../
INCLUDEPATH += ../
include("seraphlibs.pro")


SOURCES += main.cpp \
    scantostlmcu.cpp

HEADERS += \
    scantostlmcu.h
