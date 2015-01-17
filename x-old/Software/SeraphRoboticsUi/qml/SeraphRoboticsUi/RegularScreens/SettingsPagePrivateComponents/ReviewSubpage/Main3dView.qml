import QtQuick 2.4

import "../../../"
import "../../../Components"

import "../" 1.0

PageTemplate {

    ImageBasedButton {
        width : sourceSize.width
        height : sourceSize.height

        smooth: true
        anchors {
            right : parent.right
            rightMargin : 20
            top : parent.top
            topMargin: 20
        }

        onCustomClicked:
        {
            stateManager.setState(stateManager.previousState)
        }
    }

    Rectangle {
        id: view3dRect

        anchors.centerIn: parent
        width: parent.width * 0.8
        height: parent.height * 0.8

        radius: width * 0.01

        border.color: "black"
        border.width: 2

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#515151" }
            GradientStop { position: 1.0; color: "#DADADA" }
        }

        View3dPerspective {
            id: viewPersp

            anchors.fill: parent
            anchors.margins: parent.border.width

            mesh0Source: SettingsPageComponentsSettings.m_StlFilePath0
            mesh1Source: SettingsPageComponentsSettings.m_StlFilePath1

            meshColor: "white"
            fillColor: "transparent"

            navigation: true

            Component.onCompleted: {
                console.log("view 1 "+mesh0Source)
                console.log("view 2 "+mesh1Source)
            }
        }
    }

    Rectangle {
        anchors.horizontalCenter: view3dRect.horizontalCenter
        anchors.bottom: view3dRect.top
        anchors.top: parent.top
        width: view3dRect.width / 3
        color: "transparent"

        Text {
            anchors.fill: parent
            color: "black"
            font.pixelSize: parent.height / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignBottom
            text: "3D Render View"
        }
    }
}
