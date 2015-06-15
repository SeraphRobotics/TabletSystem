import QtQuick 2.4

import ".." 1.0

Rectangle {
    id : mainWindow

    radius : 10

    property alias title: title.text
    property alias header: header

    property int index : 0
    property int hideDurationMs: 600
    property int showDurationMs: 600

    signal headerPressed()

    state : "up"

    color : "transparent"

    border {
        width : 1
        color : "#cccccc"
    }

    gradient:  Gradient {
        GradientStop { position: 0.0; color: "#4b4b9b" }
        GradientStop { position: 0.5; color: "#4b4b9b" }
        GradientStop { position: 1.0; color: "#4b4b9b" }
    }

    Rectangle {
        id : background

        radius : 10
        smooth : true

        height : parent.height-30
        width : parent.width

        anchors {
            top : parent.top
            topMargin: 30
        }

        border {
            width : 1
            color : "#cccccc"
        }

        gradient:  Gradient {
            GradientStop { position: 0.0; color: "#ffffff" }
            GradientStop { position: 1.0; color: "#ebebec" }
        }
    }

    Rectangle {
        id : header

        radius : 10

        anchors {
            left : parent.left
            leftMargin: 1
            top : parent.top
            topMargin: 10
        }

        gradient:  Gradient {
            GradientStop { position: 0.0; color: "#4b4b9b" }
            GradientStop { position: 0.5; color: "#24265a" }
            GradientStop { position: 1.0; color: "#4a5ca8" }
        }

        Text {
            id : title

            text : "Choose user"

            color : "white"
            font {
                pixelSize: 20
            }

            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -5
                left : parent.left
                leftMargin: 10
            }
        }

        width : parent.width-2
        height : 30

        Image {
            anchors {
                right : parent.right
                rightMargin: 20
                verticalCenter: parent.verticalCenter
            }

            smooth : true
            source : "qrc:/QmlResources/arrow-down.png"
        }

        MouseArea {
            anchors.fill: parent

            onClicked: {
                headerPressed()
            }
        }
    }

    states: [
        State {
            name: "up"
            PropertyChanges { target: header; radius:10}
            PropertyChanges { target: header; height :extensibleAreaUpHeight}
            PropertyChanges { target: mainWindow; height :extensibleAreaUpHeight}

        },
        State {
            id : downState

            name: "down"

            PropertyChanges { target: header; radius:0}
            PropertyChanges { target: header; height :extensibleAreaUpHeight}
            PropertyChanges { target: mainWindow;
                height : SettingsPageComponentsSettings.m_MainRootObject.height
                         -mainWindow.y
                         -70
                         -index*
                         (extensibleAreaUpHeight+extensibleAreaSpaceBeetwen)
            }
        }
    ]

    transitions: [
        Transition {
            from: "down"; to: "up";
            ParallelAnimation {
                NumberAnimation {properties: "height"; duration: hideDurationMs
                    easing.type: Easing.Linear}
            }
        },
        Transition {
            from: "up"; to: "down";
            ParallelAnimation {
                NumberAnimation {properties: "height"; duration:showDurationMs
                    easing.type: Easing.Linear}
            }
        }
    ]
}
