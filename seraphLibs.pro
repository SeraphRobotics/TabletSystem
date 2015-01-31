# CONFIG += qt warn_off debug
# QT       += network quick sql xml concurrent
QT     += core gui xml concurrent
QT     += widgets   # for QMessageBox
CONFIG += c++11

##############################
# static libs
#CONFIG += staticlib
# ***
TARGET = seraphLibs
TEMPLATE = lib
VERSION = 0.0.01

# stand alone
#TEMPLATE = app
#CONFIG   += console
#CONFIG   -= app_bundle
##############################

# shared libs
DEFINES += SERAPHLIBS_LIBRARY
# DEFINES += SW_VERSION=\\\"\"0.5\"\\\"
# DEFINES += SW_VERSION=\\\"\"$$PWD\"\\\"
# DEFINES += SW_VERSION=$$PWD\;



win32:LIBS += -LC:\\OpenCV\\build_with_Qt\\release\\lib \
    -lopencv_calib3d245.dll\
    -lopencv_contrib245.dll\
    -lopencv_core245.dll \
    -lopencv_features2d245.dll \
    -lopencv_flann245.dll \
    -lopencv_gpu245.dll \
    -lopencv_highgui245.dll \
    -lopencv_imgproc245.dll \
    -lopencv_legacy245.dll \
    -lopencv_ml245.dll\
    -lopencv_objdetect245.dll \
    -lopencv_ts245 \
    -lopencv_video245.dll

unix:LIBS += \
    -lopencv_calib3d \
    -lopencv_contrib \
    -lopencv_core \
    -lopencv_features2d \
    -lopencv_flann \
    -lopencv_gpu \
    -lopencv_highgui \
    -lopencv_imgproc \
    -lopencv_legacy \
    -lopencv_ml \
    -lopencv_objdetect \
    -lopencv_ts \
    -lopencv_video

unix:LIBS += -lz

win32:INCLUDEPATH += "C:\\Eigen\\include\\eigen3"

# run test suit,
# clone a build in projects and add build additional args of CONFIG+=test to run below block
# or run 'qmake CONFIG+=test' on source directory, followed by make.
test {
    message(Test suite build)
    QT += testlib
    QT += widgets
    TARGET = tests

    TEMPLATE = app
    CONFIG   += console

    # SOURCES -= main.cpp

    SOURCES += \
    tests/main.cpp \
    tests/testscanmanger.cpp

    HEADERS += \
    tests/testscanmanger.h

} else {
    message(shared libs build)
}

HEADERS += \
    Controllers/gcodecontroller.h \
    Controllers/mergecontroller.h \
    Controllers/orthoticscontroller.h \
    Controllers/padgenerator.h \
    Controllers/printjobcontroller.h \
    Controllers/processingfunctions.h \
    Controllers/repaircontroller.h \
    Controllers/scandataprocesser.h \
    Controllers/scannerfunctions.h \
    Controllers/scanprocessing.h \
    Controllers/slicercontroller.h \
    Controllers/stlgeneration.h \
    Controllers/topcoatcontroller.h \
    DataStructures/usbmanager.h \
    DataStructures/usbminder.h \
    DataStructures/user.h \
    DataStructures/usermanager.h \
    DataStructures/xygrid.h \
    DataStructures/basicstructures.h \
    DataStructures/manipulations.h \
    DataStructures/orthotic.h \
    DataStructures/orthoticmanager.h \
    DataStructures/patientdata.h \
    DataStructures/patientmanager.h \
    DataStructures/printingstructs.h \
    DataStructures/scan.h \
    DataStructures/scanmanager.h \
    libraries/shared/amf/amffile.h \
    libraries/shared/amf/amfmesh.h \
    libraries/shared/amf/amfobject.h \
    libraries/shared/amf/amfregion.h \
    libraries/shared/amf/amftriangle.h \
    libraries/shared/amf/amfvertex.h \
    libraries/shared/amf/edgekey.h \
    libraries/shared/math/float.h \
    libraries/shared/math/line.h \
    libraries/shared/math/loopinxyplane.h \
    libraries/shared/math/matrix4x4.h \
    libraries/shared/math/microntolerance.h \
    libraries/shared/math/plane.h \
    libraries/shared/math/quaternion.h \
    libraries/shared/math/sphere.h \
    libraries/shared/math/structs.h \
    libraries/shared/math/triangle.h \
    libraries/shared/math/vector3.h \
    libraries/shared/stl/stlfacet.h \
    libraries/shared/stl/stlfile.h \
    libraries/shared/stl/stlmesh.h \
    libraries/shared/fah-constants.h \
    libraries/common/quazip/crypt.h \
    libraries/common/quazip/ioapi.h \
    libraries/common/quazip/quazip.h \
    libraries/common/quazip/quazipfile.h \
    libraries/common/quazip/quazipfileinfo.h \
    libraries/common/quazip/quazipnewinfo.h \
    libraries/common/quazip/unzip.h \
    libraries/common/quazip/zip.h \
    libraries/common/confirm.h \
    libraries/common/tinman.h \
    libraries/common/todo.h \
    libraries/qdevicewatcher/src/qdevicewatcher_p.h \
    libraries/qdevicewatcher/src/qdevicewatcher.h \
    libraries/qdevicewatcher/src/qdevicewatcher_global.h \
    libraries/qdevicewatcher/src/qdevicechangeevent.h \
    globals.h \
    runtime/mastercontrolunit.h \
    View/UI_structs.h

SOURCES += \
    Controllers/gcodecontroller.cpp \
    Controllers/mergecontroller.cpp \
    Controllers/orthoticscontroller.cpp \
    Controllers/padgenerator.cpp \
    Controllers/printjobcontroller.cpp \
    Controllers/processingfunctions.cpp \
    Controllers/repaircontroller.cpp \
    Controllers/scandataprocesser.cpp \
    Controllers/scannerfunctions.cpp \
    Controllers/scanprocessing.cpp \
    Controllers/slicercontroller.cpp \
    Controllers/stlgeneration.cpp \
    Controllers/topcoatcontroller.cpp \
    DataStructures/usbmanager.cpp \
    DataStructures/usbminder.cpp \
    DataStructures/user.cpp \
    DataStructures/usermanager.cpp \
    DataStructures/xygrid.cpp \
    DataStructures/basicstructures.cpp \
    DataStructures/manipulations.cpp \
    DataStructures/orthotic.cpp \
    DataStructures/orthoticmanager.cpp \
    DataStructures/patientdata.cpp \
    DataStructures/patientmanager.cpp \
    DataStructures/printingstructs.cpp \
    DataStructures/scan.cpp \
    DataStructures/scanmanager.cpp \
    libraries/shared/amf/amffile.cpp \
    libraries/shared/amf/amfmesh.cpp \
    libraries/shared/amf/amfobject.cpp \
    libraries/shared/amf/amfregion.cpp \
    libraries/shared/amf/amftriangle.cpp \
    libraries/shared/amf/amfvertex.cpp \
    libraries/shared/amf/edgekey.cpp \
    libraries/shared/math/float.cpp \
    libraries/shared/math/line.cpp \
    libraries/shared/math/loopinxyplane.cpp \
    libraries/shared/math/matrix4x4.cpp \
    libraries/shared/math/microntolerance.cpp \
    libraries/shared/math/plane.cpp \
    libraries/shared/math/quaternion.cpp \
    libraries/shared/math/sphere.cpp \
    libraries/shared/math/triangle.cpp \
    libraries/shared/math/vector3.cpp \
    libraries/shared/stl/stlfile.cpp \
    libraries/shared/stl/stlmesh.cpp \
    libraries/common/quazip/quazip.cpp \
    libraries/common/quazip/quazipfile.cpp \
    libraries/common/quazip/quazipnewinfo.cpp \
    libraries/common/quazip/ioapi.c \
    libraries/common/quazip/unzip.c \
    libraries/common/quazip/zip.c \
    libraries/common/confirm.cpp \
    libraries/common/tinman.cpp \
    libraries/common/todo.cpp \
    libraries/qdevicewatcher/src/qdevicewatcher.cpp \
    libraries/qdevicewatcher/src/qdevicechangeevent.cpp \
    runtime/mastercontrolunit.cpp \
    View/UI_structs.cpp \
    globals.cpp





unix:  SOURCES += libraries/qdevicewatcher/src/qdevicewatcher_linux.cpp
win32 {
    wince*: SOURCES += libraries/qdevicewatcher/src/qdevicewatcher_wince.cpp
    else:  SOURCES += libraries/qdevicewatcher/src/qdevicewatcher_win32.cpp
}
macx {
    LIBS += -framework DiskArbitration -framework Foundation
    SOURCES += libraries/qdevicewatcher/src/qdevicewatcher_mac.cpp
}







