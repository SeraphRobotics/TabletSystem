# Add more folders to ship with the application, here
folder_01.source = qml/SeraphRoboticsUi
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += xml 3dquick core

QMAKE_CXXFLAGS += -std=c++11

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    src/QmlPageStateManager.cpp \
    ../SeraphRoboticsBackend/UI_tester/mastercontrolunit.cpp \
    ../SeraphRoboticsBackend/shared/stl/stlfile.cpp \
    ../SeraphRoboticsBackend/shared/stl/stlmesh.cpp \
    ../SeraphRoboticsBackend/DataStructures/basicstructures.cpp \
    ../SeraphRoboticsBackend/DataStructures/manipulations.cpp \
    ../SeraphRoboticsBackend/DataStructures/orthotic.cpp \
    ../SeraphRoboticsBackend/DataStructures/orthoticmanager.cpp \
    ../SeraphRoboticsBackend/DataStructures/patientdata.cpp \
    ../SeraphRoboticsBackend/DataStructures/patientmanager.cpp \
    ../SeraphRoboticsBackend/DataStructures/scan.cpp \
    ../SeraphRoboticsBackend/DataStructures/scanmanager.cpp \
    ../SeraphRoboticsBackend/DataStructures/test.cpp \
    ../SeraphRoboticsBackend/DataStructures/user.cpp \
    ../SeraphRoboticsBackend/DataStructures/usermanager.cpp \
    ../SeraphRoboticsBackend/DataStructures/xygrid.cpp \
    ../SeraphRoboticsBackend/shared/math/float.cpp \
    ../SeraphRoboticsBackend/shared/math/line.cpp \
    ../SeraphRoboticsBackend/shared/math/loopinxyplane.cpp \
    ../SeraphRoboticsBackend/shared/math/matrix4x4.cpp \
    ../SeraphRoboticsBackend/shared/math/microntolerance.cpp \
    ../SeraphRoboticsBackend/shared/math/plane.cpp \
    ../SeraphRoboticsBackend/shared/math/quaternion.cpp \
    ../SeraphRoboticsBackend/shared/math/sphere.cpp \
    ../SeraphRoboticsBackend/shared/math/triangle.cpp \
    ../SeraphRoboticsBackend/shared/math/vector3.cpp \
    ../SeraphRoboticsBackend/shared/amf/amffile.cpp \
    ../SeraphRoboticsBackend/shared/amf/amfmesh.cpp \
    ../SeraphRoboticsBackend/shared/amf/amfobject.cpp \
    ../SeraphRoboticsBackend/shared/amf/amfregion.cpp \
    ../SeraphRoboticsBackend/shared/amf/amftriangle.cpp \
    ../SeraphRoboticsBackend/shared/amf/amfvertex.cpp \
    ../SeraphRoboticsBackend/shared/amf/edgekey.cpp \
    src/QmlCppWrapper.cpp \
    src/Models/UserObject.cpp \
    src/Models/PatientObject.cpp \
    src/Models/NameObject.cpp \
    src/Models/UsersListModel.cpp \
    src/Models/PatientsListModel.cpp \
    src/ApplicationSettingsManager.cpp \
    src/Models/PatientDataObject.cpp \
    src/SettingsPageManager.cpp \
    src/XmlManager/XmlFileManager.cpp \
    src/3dHelper/q3dhelper.cpp \
    src/ImageProcessingClass/ImageContoursDetector.cpp



INCLUDEPATH += ../SeraphRoboticsBackend/ \
               ../SeraphRoboticsBackend/DataStructures/ \
               ../SeraphRoboticsBackend/UI_tester/ \
                ../src \
                src/ \
                src/XmlManager/

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    src/QmlPageStateManager.h \
    ../SeraphRoboticsBackend/UI_tester/mastercontrolunit.h \
    ../SeraphRoboticsBackend/shared/stl/stlfacet.h \
    ../SeraphRoboticsBackend/shared/stl/stlfile.h \
    ../SeraphRoboticsBackend/shared/stl/STLMesh.h \
    ../SeraphRoboticsBackend/DataStructures/basicstructures.h \
    ../SeraphRoboticsBackend/DataStructures/manipulations.h \
    ../SeraphRoboticsBackend/DataStructures/orthotic.h \
    ../SeraphRoboticsBackend/DataStructures/orthoticmanager.h \
    ../SeraphRoboticsBackend/DataStructures/patientdata.h \
    ../SeraphRoboticsBackend/DataStructures/patientmanager.h \
    ../SeraphRoboticsBackend/DataStructures/scan.h \
    ../SeraphRoboticsBackend/DataStructures/scanmanager.h \
    ../SeraphRoboticsBackend/DataStructures/test.h \
    ../SeraphRoboticsBackend/DataStructures/user.h \
    ../SeraphRoboticsBackend/DataStructures/usermanager.h \
    ../SeraphRoboticsBackend/DataStructures/xygrid.h \
    ../SeraphRoboticsBackend/shared/math/float.h \
    ../SeraphRoboticsBackend/shared/math/line.h \
    ../SeraphRoboticsBackend/shared/math/loopinxyplane.h \
    ../SeraphRoboticsBackend/shared/math/matrix4x4.h \
    ../SeraphRoboticsBackend/shared/math/microntolerance.h \
    ../SeraphRoboticsBackend/shared/math/plane.h \
    ../SeraphRoboticsBackend/shared/math/quaternion.h \
    ../SeraphRoboticsBackend/shared/math/sphere.h \
    ../SeraphRoboticsBackend/shared/math/structs.h \
    ../SeraphRoboticsBackend/shared/math/triangle.h \
    ../SeraphRoboticsBackend/shared/math/vector3.h \
    ../SeraphRoboticsBackend/shared/amf/amffile.h \
    ../SeraphRoboticsBackend/shared/amf/amfmesh.h \
    ../SeraphRoboticsBackend/shared/amf/amfobject.h \
    ../SeraphRoboticsBackend/shared/amf/amfregion.h \
    ../SeraphRoboticsBackend/shared/amf/amftriangle.h \
    ../SeraphRoboticsBackend/shared/amf/amfvertex.h \
    ../SeraphRoboticsBackend/shared/amf/edgekey.h \
    src/QmlCppWrapper.h \
    src/Models/UserObject.h \
    src/Models/PatientObject.h \
    src/Models/NameObject.h \
    src/Models/UsersListModel.h \
    src/Models/PatientsListModel.h \
    src/ApplicationSettingsManager.h \
    src/Models/PatientDataObject.h \
    src/SettingsPageManager.h \
    src/XmlManager/XmlFileManager.h \
    src/3dHelper/q3dhelper.h \
    src/ImageProcessingClass/ImageContoursDetector.h


RESOURCES += \
    resourcesFile.qrc


##This part is for building exe with administrator acess, use only for build exe for user.

#win32 {
#CONFIG += SeraphRoboticsUi.exe.manifest
#QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
#}

win32:RC_FILE = SeraphRoboticsUi.rc
win32:CONFIG(release, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_core249
else:win32:CONFIG(debug, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_core249d

INCLUDEPATH += $$PWD/../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../opencv/build/include

win32:CONFIG(release, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_imgproc249
else:win32:CONFIG(debug, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_imgproc249d

INCLUDEPATH += $$PWD/../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../opencv/build/include

win32:CONFIG(release, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_highgui249
else:win32:CONFIG(debug, debug|release): LIBS +=-L$$PWD/../../../../opencv/build/x86/vc12/lib/ -lopencv_highgui249d

INCLUDEPATH += $$PWD/../../../../opencv/build/include
DEPENDPATH += $$PWD/../../../../opencv/build/include
