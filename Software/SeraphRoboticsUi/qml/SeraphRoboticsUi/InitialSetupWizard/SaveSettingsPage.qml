import QtQuick 2.4

import "../Components"
import ".."

Rectangle {

    anchors.fill: parent

    gradient: Gradient {
        GradientStop { position: 0.0; color: "#ffffff" }
        GradientStop { position: 1.0; color: "#999999" }
    }

    BlackDescriptionText {
        id : infoText

        opacity : 0

        anchors {
            centerIn: parent
        }

        text : "<p align=\"center\">In the future, you can adjust these settings<br/>
by clicking the “SR” icon below.<br/></p>"
    }


    BlackDescriptionText {
        id : detailedText

        opacity : 0
        anchors {
            top : infoText.bottom
            horizontalCenter:  infoText.horizontalCenter
        }

        text : "<p align=\"center\"
</br>Click the “SR” icon now to review and save<br/>
your settings and continue.</p>"
    }

    BlueArrowComponent {
        id : blueArrowElement

        transform: Rotation { angle: 90 }

        anchors {
            bottom : parent.bottom
            bottomMargin: 214
            right : parent.right
            rightMargin: 135
        }
    }

    Image {
        id : logo

        anchors {
            bottom : parent.bottom
            bottomMargin : 10
            right : parent.right
            rightMargin : 10
        }

        height : 85
        width : 71

        smooth : true
        source : "qrc:/QmlResources/Sr_logo.png"

        MouseArea {
            anchors.fill: parent

            cursorShape: Qt.PointingHandCursor

            onClicked: {
                if(fadeIn.running === false)
                    stateManager.setState("summaryPage")
            }
        }
    }

        SequentialAnimation {
            id : fadeIn

            running: true

            NumberAnimation { target: infoText; property: "opacity"; to: 1; duration: 2000 }
            NumberAnimation { target: detailedText; property: "opacity"; to: 1; duration: 2000 }
            NumberAnimation {  target: blueArrowElement; property: "opacity"; to: 1; duration: 1000 }

            onRunningChanged: {
                if(running === false)
                {
                    blueArrowElement.startBlinking()
                }
            }
        }
}
