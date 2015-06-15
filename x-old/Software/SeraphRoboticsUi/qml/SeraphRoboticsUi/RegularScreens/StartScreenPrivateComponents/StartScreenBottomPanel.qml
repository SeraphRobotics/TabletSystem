import QtQuick 2.4
import "../../Components"
import "../" 1.0

// Bottom panel from page 12.
Row {
    property Item rootVisibleElement: null

    spacing : 100

    ImageBasedButton {
        id : alertIcon

        state : "NORMAL"
        source : "qrc:/QmlResources/no-errorsAlertUpdate.png"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        states: [
                State {
                    name: "NORMAL"
                    PropertyChanges { target: alertIcon;
                        source:"qrc:/QmlResources/no-errorsAlertUpdate.png"}

                },
                State {
                    name: "ALERT"
                    PropertyChanges { target: alertIcon;
                        source: "qrc:/QmlResources/errors.png"}
                }
            ]
        onCustomClicked: {
            if(state == "NORMAL")
                state = "ALERT"
            else if( state == "ALERT")
                state = "NORMAL"
        }
    }

    ImageBasedButton {
    source : "qrc:/QmlResources/camera.png"
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 2
        onCustomClicked : {
            FramePopup.showPopup()
        }
    }

    ImageBasedButton {
    source : "qrc:/QmlResources/patient.png"
    anchors.bottom: parent.bottom
        onCustomClicked : {
            FramePopup.showPopup()
        }
    }
}
