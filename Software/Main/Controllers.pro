#TEMPLATE = subdirs
#CONFIG += ordered
SUBDIRS = libqdevicewatcher usbminder

libqdevicewatcher.file = ../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pro

usbminder.file = USBMinder.pro
usbminder.depends += libqdevicewatcher

