QT     += core gui xml concurrent
QT     += widgets   # for QMessageBox
QT     += serialport
CONFIG += c++11

CONFIG += opencv2411
CONFIG += qt54

qt54{
 win32:INCLUDEPATH +=C:\\Qt5\\5.4\\Src\\qtbase\\src\\3rdparty\\zlib
}

opencv2411{
    win32:INCLUDEPATH += C:\\opencv\\build_with_Qt5\\release\\include
    win32:LIBS += -LC:\\opencv\\build_with_Qt5\\release\\x64\\mingw\\lib \
        -lopencv_calib3d2411.dll\
        -lopencv_contrib2411.dll\
        -lopencv_core2411.dll \
        -lopencv_features2d2411.dll \
        -lopencv_flann2411.dll \
        -lopencv_gpu2411.dll \
        -lopencv_highgui2411.dll \
        -lopencv_imgproc2411.dll \
        -lopencv_legacy2411.dll \
        -lopencv_ml2411.dll\
        -lopencv_objdetect2411.dll \
        -lopencv_ts2411 \
        -lopencv_video2411.dll
}

win32:INCLUDEPATH +="C:\\Eigen\\eigen3"



HEADERS += \
    ../../Controllers/gcodecontroller.h \
    ../../Controllers/mergecontroller.h \
    ../../Controllers/orthoticscontroller.h \
    ../../Controllers/padgenerator.h \
    ../../Controllers/printjobcontroller.h \
    ../../Controllers/processingfunctions.h \
    ../../Controllers/repaircontroller.h \
    ../../Controllers/scandataprocesser.h \
    ../../Controllers/scannerfunctions.h \
    ../../Controllers/scanprocessing.h \
    ../../Controllers/slicercontroller.h \
    ../../Controllers/stlgeneration.h \
    ../../Controllers/topcoatcontroller.h \
    ../../Controllers/scannercontroller.h\
    ../../Controllers/scannerwatcher.h\
    ../../DataStructures/usbmanager.h \
    ../../DataStructures/usbminder.h \
    ../../DataStructures/user.h \
    ../../DataStructures/usermanager.h \
    ../../DataStructures/xygrid.h \
    ../../DataStructures/basicstructures.h \
    ../../DataStructures/manipulations.h \
    ../../DataStructures/orthotic.h \
    ../../DataStructures/orthoticmanager.h \
    ../../DataStructures/patientdata.h \
    ../../DataStructures/patientmanager.h \
    ../../DataStructures/printingstructs.h \
    ../../DataStructures/scan.h \
    ../../DataStructures/scanmanager.h \
    ../../DataStructures/recordkeeper.h\
    ../../DataStructures/scannerarduinointerface.h\
    ../../libraries/shared/amf/amffile.h \
    ../../libraries/shared/amf/amfmesh.h \
    ../../libraries/shared/amf/amfobject.h \
    ../../libraries/shared/amf/amfregion.h \
    ../../libraries/shared/amf/amftriangle.h \
    ../../libraries/shared/amf/amfvertex.h \
    ../../libraries/shared/amf/edgekey.h \
    ../../libraries/shared/math/float.h \
    ../../libraries/shared/math/line.h \
    ../../libraries/shared/math/loopinxyplane.h \
    ../../libraries/shared/math/matrix4x4.h \
    ../../libraries/shared/math/microntolerance.h \
    ../../libraries/shared/math/plane.h \
    ../../libraries/shared/math/quaternion.h \
    ../../libraries/shared/math/sphere.h \
    ../../libraries/shared/math/structs.h \
    ../../libraries/shared/math/triangle.h \
    ../../libraries/shared/math/vector3.h \
    ../../libraries/shared/stl/stlfacet.h \
    ../../libraries/shared/stl/stlfile.h \
    ../../libraries/shared/stl/stlmesh.h \
    ../../libraries/shared/fah-constants.h \
    ../../libraries/common/quazip/crypt.h \
    ../../libraries/common/quazip/ioapi.h \
    ../../libraries/common/quazip/quazip.h \
    ../../libraries/common/quazip/quazipfile.h \
    ../../libraries/common/quazip/quazipfileinfo.h \
    ../../libraries/common/quazip/quazipnewinfo.h \
    ../../libraries/common/quazip/unzip.h \
    ../../libraries/common/quazip/zip.h \
    ../../libraries/common/confirm.h \
    ../../libraries/common/tinman.h \
    ../../libraries/common/todo.h \
    ../../libraries/qdevicewatcher/src/qdevicewatcher_p.h \
    ../../libraries/qdevicewatcher/src/qdevicewatcher.h \
    ../../libraries/qdevicewatcher/src/qdevicewatcher_global.h \
    ../../libraries/qdevicewatcher/src/qdevicechangeevent.h \
    ../../globals.h \
    ../../runtime/mastercontrolunit.h \
    ../../View/UI_structs.h

SOURCES += \
    ../../Controllers/gcodecontroller.cpp \
    ../../Controllers/mergecontroller.cpp \
    ../../Controllers/orthoticscontroller.cpp \
    ../../Controllers/padgenerator.cpp \
    ../../Controllers/printjobcontroller.cpp \
    ../../Controllers/processingfunctions.cpp \
    ../../Controllers/repaircontroller.cpp \
    ../../Controllers/scandataprocesser.cpp \
    ../../Controllers/scannerfunctions.cpp \
    ../../Controllers/scanprocessing.cpp \
    ../../Controllers/slicercontroller.cpp \
    ../../Controllers/stlgeneration.cpp \
    ../../Controllers/topcoatcontroller.cpp \
    ../../Controllers/scannercontroller.cpp\
    ../../Controllers/scannerwatcher.cpp\
    ../../DataStructures/usbmanager.cpp \
    ../../DataStructures/usbminder.cpp \
    ../../DataStructures/user.cpp \
    ../../DataStructures/usermanager.cpp \
    ../../DataStructures/xygrid.cpp \
    ../../DataStructures/basicstructures.cpp \
    ../../DataStructures/manipulations.cpp \
    ../../DataStructures/orthotic.cpp \
    ../../DataStructures/orthoticmanager.cpp \
    ../../DataStructures/patientdata.cpp \
    ../../DataStructures/patientmanager.cpp \
    ../../DataStructures/printingstructs.cpp \
    ../../DataStructures/scan.cpp \
    ../../DataStructures/scanmanager.cpp \
    ../../DataStructures/recordkeeper.cpp\
    ../../DataStructures/scannerarduinointerface.cpp\
    ../../libraries/shared/amf/amffile.cpp \
    ../../libraries/shared/amf/amfmesh.cpp \
    ../../libraries/shared/amf/amfobject.cpp \
    ../../libraries/shared/amf/amfregion.cpp \
    ../../libraries/shared/amf/amftriangle.cpp \
    ../../libraries/shared/amf/amfvertex.cpp \
    ../../libraries/shared/amf/edgekey.cpp \
    ../../libraries/shared/math/float.cpp \
    ../../libraries/shared/math/line.cpp \
    ../../libraries/shared/math/loopinxyplane.cpp \
    ../../libraries/shared/math/matrix4x4.cpp \
    ../../libraries/shared/math/microntolerance.cpp \
    ../../libraries/shared/math/plane.cpp \
    ../../libraries/shared/math/quaternion.cpp \
    ../../libraries/shared/math/sphere.cpp \
    ../../libraries/shared/math/triangle.cpp \
    ../../libraries/shared/math/vector3.cpp \
    ../../libraries/shared/stl/stlfile.cpp \
    ../../libraries/shared/stl/stlmesh.cpp \
    ../../libraries/common/quazip/quazip.cpp \
    ../../libraries/common/quazip/quazipfile.cpp \
    ../../libraries/common/quazip/quazipnewinfo.cpp \
    ../../libraries/common/quazip/ioapi.c \
    ../../libraries/common/quazip/unzip.c \
    ../../libraries/common/quazip/zip.c \
    ../../libraries/common/confirm.cpp \
    ../../libraries/common/tinman.cpp \
    ../../libraries/common/todo.cpp \
    ../../libraries/qdevicewatcher/src/qdevicewatcher.cpp \
    ../../libraries/qdevicewatcher/src/qdevicechangeevent.cpp \
    ../../runtime/mastercontrolunit.cpp \
    ../../View/UI_structs.cpp \
    ../../globals.cpp





unix:  SOURCES += ../../libraries/qdevicewatcher/src/qdevicewatcher_linux.cpp
win32 {
    wince*: SOURCES += ../../libraries/qdevicewatcher/src/qdevicewatcher_wince.cpp
    else:  SOURCES += ../../libraries/qdevicewatcher/src/qdevicewatcher_win32.cpp
}







