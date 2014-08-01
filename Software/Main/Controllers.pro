include(../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pri)

win32:!wince*:LIBS += -lUser32

SOURCES += ../Controllers/usbminder.cpp \
    ../Controllers/usbmanager.cpp \
    ../Controllers/scanprocessing.cpp \
    ../Controllers/scandata.cpp

HEADERS += ../Controllers/usbminder.h \
    ../Controllers/usbmanager.h \
    ../Controllers/scanprocessing.h \
    ../Controllers/scandata.h
