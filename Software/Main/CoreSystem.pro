QT       += core\
            gui\
            xml

TARGET = CoreSystem
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp 


INCLUDEPATH += ../
        

include(shared-math.pro)
include(shared-stl.pro)
include(shared-amf.pro)
#include(common.pro)
include(shared.pro)
include(DataStructures.pro)
include(Controllers.pro)
include(View.pro)

include(../UnitTest/test.pro)

INCLUDEPATH += C:\\OpenCV\\build_with_Qt\\release\\include

#LIBS += -LC:\\OpenCV\\build_with_Qt\\release\\lib \
#    -lopencv_calib3d243.dll\
#    -lopencv_contrib243.dll\
#    -lopencv_core243.dll \
#    -lopencv_features2d243.dll \
#    -lopencv_flann243.dll \
#    -lopencv_gpu243.dll \
#    -lopencv_highgui243.dll \
#    -lopencv_imgproc243.dll \
#    -lopencv_legacy243.dll \
#    -lopencv_ml243.dll\
#    -lopencv_objdetect243.dll \
#    -lopencv_ts243 \
#    -lopencv_video243.dll

LIBS += -LC:\\OpenCV\\build_with_Qt\\release\\lib \
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


INCLUDEPATH += "C:\\Eigen\\include\\eigen3"
#INCLUDEPATH += "C:\\Eigen\\eigen3"
