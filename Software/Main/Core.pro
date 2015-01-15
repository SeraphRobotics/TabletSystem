#-------------------------------------------------
#
# Project created by QtCreator 2014-05-29T07:02:21
#
#-------------------------------------------------


TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libqdevicewatcher CoreSystem

libqdevicewatcher.file = ../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pro

CoreSystem.file = CoreSystem.pro
CoreSystem.depends += libqdevicewatcher

