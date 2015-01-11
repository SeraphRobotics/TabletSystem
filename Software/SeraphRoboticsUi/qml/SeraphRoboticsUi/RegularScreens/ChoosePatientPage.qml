import QtQuick 2.4
import ".."
import "../Components"
import "ChoosePatientScreenPrivateComponents"

// Page 14 from pdf.
ChoosePatientScreenMainWindow {


    ChoosePatientScreenPatientsList
    {
        id : patientsList
    }

    //Timer used to simulate usb being plugged/unplugged
    Timer {
        interval: 60000; running: true; repeat: true
        onTriggered: {
            manageUsbDrive.state = (manageUsbDrive.state == "usbNotDetected")
                    ? "usbDetected" : "usbNotDetected"
        }
    }

    GroupBoxTemplate {
        id : manageUsbDrive

        property real widthScaleValue: (700/1280)

        width : widthScaleValue*parent.width

        title: "Manage USB Drive"
        state : "usbDetected"

        anchors {
            top : parent.top
            topMargin: patientsList.anchors.topMargin
            right : parent.right
            rightMargin: 30
            bottom : parent.bottom
            bottomMargin : patientsList.anchors.bottomMargin
        }

        ChoosePatientScreenUsbDetected
        {
            id : manageUsbView
        }

        Image {
            id : noUsbDetectedImage

            width : sourceSize.width
            height : sourceSize.height

            visible : false

            anchors {
                centerIn: parent
            }

            smooth: true
            source : "qrc:/QmlResources/noUsbDetected.png"
        }

        states: [
            State {
                name: "usbDetected"
                PropertyChanges { target: manageUsbView; visible: true }
                PropertyChanges { target: noUsbDetectedImage; visible: false }
            },
            State {
                name: "usbNotDetected"
                PropertyChanges { target: manageUsbView; visible: false }
                PropertyChanges { target: noUsbDetectedImage; visible: true }
            }
        ]
    }

    rightNavigationButton.onButtonClicked:
    {
        stateManager.setState("patientHistoryScreen")
    }

}
