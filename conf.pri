# Configuration file containing pathes to all required libraries for different
# OSes for backend. 

OPENCV_INCLUDE = $$(opencv_inc)
INCLUDEPATH += "$$OPENCV_INCLUDE"

test {
  OPENCV_LIBS = $$(opencv_lib)
  isEmpty(OPENCV_INCLUDE) | isEmpty(OPENCV_LIBS) {
    error(You didn\'t specify 3rd party opencv include and/or library. \
          Please see https://git.milosolutions.com/qt/seraphrobotics/wikis/build)
  }

  LIBS += -L"$$OPENCV_LIBS"

  win32 {
    # Set opencv library version suffix
    opencv2411 {
      OPENCV_VER = "2411"
    }
    opencv245 {
      OPENCV_VER = "245"
    }
    opencv243 {
      OPENCV_VER = "243"
    }

    LIBS += \
        -lopencv_calib3d"$$OPENCV_VER" \
        -lopencv_contrib"$$OPENCV_VER" \
        -lopencv_core"$$OPENCV_VER" \
        -lopencv_features2d"$$OPENCV_VER" \
        -lopencv_flann"$$OPENCV_VER" \
        -lopencv_gpu"$$OPENCV_VER" \
        -lopencv_highgui"$$OPENCV_VER" \
        -lopencv_imgproc"$$OPENCV_VER" \
        -lopencv_legacy"$$OPENCV_VER" \
        -lopencv_ml"$$OPENCV_VER" \
        -lopencv_objdetect"$$OPENCV_VER" \
        -lopencv_ts"$$OPENCV_VER" \
        -lopencv_video"$$OPENCV_VER"
  }

  unix {
    LIBS += \
        -lopencv_core \
        -lopencv_highgui \
        -lopencv_imgproc
  }
}

EIGEN_INCLUDE = $$(eigen_inc)
isEmpty(EIGEN_INCLUDE) {
  error(You didn\'t specify 3rd party EIGEN include. \
        Please see https://git.milosolutions.com/qt/seraphrobotics/wikis/build)
}

INCLUDEPATH += "$$EIGEN_INCLUDE"

LIBS += -lz

win32 {
  message(Using Qt $$QT_MAJOR_VERSION)
  greaterThan(QT_MAJOR_VERSION, 4) {
  # In Qt 5+ we can use #include <QtZlib/zlib.h>
  } else {
    INCLUDEPATH += $$[QT_INSTALL_PREFIX]\\src\\3rdparty\\zlib
  }
}

# Instructions to keep build directories clean
CONFIG(release, debug|release) {
  DESTDIR = release
  OBJECTS_DIR = release/.obj
  MOC_DIR = release/.moc
  RCC_DIR = release/.rcc
  UI_DIR = release/.ui
}
CONFIG(debug, debug|release) {
  DESTDIR = debug
  OBJECTS_DIR = debug/.obj
  MOC_DIR = debug/.moc
  RCC_DIR = debug/.rcc
  UI_DIR = debug/.ui
}

message(---)
message(OPENCV_INCLUDE: $$OPENCV_INCLUDE)
message(OPENCV_LIBS: $$OPENCV_LIBS)
message(EIGEN_INCLUDE: $$EIGEN_INCLUDE)
message(Libs: $$LIBS)
message(Includes: $$INCLUDEPATH)
message(---)
