TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libqdevicewatcher scanerminder

libqdevicewatcher.file = qdevicewatcher-master/src/libQDeviceWatcher.pro

scanerminder.file = ScannerMinder/ScannerMinder.pro
scanerminder.depends += libqdevicewatcher

