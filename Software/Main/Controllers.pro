include(../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pri)

win32:!wince*:LIBS += -lUser32

SOURCES += ../Controllers/usbminder.cpp \
    ../Controllers/usbmanager.cpp

HEADERS += ../Controllers/usbminder.h \
    ../Controllers/usbmanager.h
