include(../libraries/qdevicewatcher-master/src/libQDeviceWatcher.pri)

win32:!wince*:LIBS += -lUser32

SOURCES += ../Controllers/scanprocessing.cpp \
    ../Controllers/scandataprocesser.cpp \
    ../Controllers/processingfunctions.cpp \
    ../Controllers/orthoticscontroller.cpp \
    ../Controllers/stlgeneration.cpp\
    ../Controllers/scannerfunctions.cpp

HEADERS += ../Controllers/scanprocessing.h \
    ../Controllers/scandataprocesser.h \
    ../Controllers/processingfunctions.h \
    ../Controllers/orthoticscontroller.h \
    ../Controllers/stlgeneration.h \
    ../Controllers/scannerfunctions.h
