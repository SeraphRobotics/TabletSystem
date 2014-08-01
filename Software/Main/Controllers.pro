include(../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pri)

win32:!wince*:LIBS += -lUser32

SOURCES += ../Controllers/scanprocessing.cpp \
    ../Controllers/scandataprocesser.cpp

HEADERS += ../Controllers/scanprocessing.h \
    ../Controllers/scandataprocesser.h
