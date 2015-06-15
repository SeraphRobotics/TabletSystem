//PostingBox.qml
import QtQuick 2.4
import QtQuick.Controls 1.2

import "../"

Rectangle {
    id: postingBox

    property int cellWidth: width / 3
    property int varusVal: 0
    property int verticleVal: 0

    state: "activeState"
    color: "transparent"
    y: parent.height / 2; x: 0
    width: parent.width; height: parent.height / 2

    Rectangle {
        id: textRect1

        color: "transparent"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: height / 2
        width: parent.cellWidth - (anchors.margins * 2)
        height: parent.height / 5

        Text {
            id: text1

            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height / 2

            Behavior on color {
                ColorAnimation {}
            }
        }
    }

    Rectangle {
        id: textRect2

        color: "transparent"
        anchors.left: textRect1.right
        anchors.top: parent.top
        anchors.topMargin: height / 2
        anchors.leftMargin: anchors.topMargin * 2
        width: parent.cellWidth - (anchors.leftMargin)
        height: parent.height / 5

        Text {
            id: text2

            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height / 2

            Behavior on color {
                ColorAnimation {}
            }
        }
    }

    Rectangle {
        id: textRect3

        color: "transparent"
        anchors.left: textRect2.right
        anchors.top: parent.top
        anchors.topMargin: height / 2
        anchors.leftMargin: anchors.topMargin * 2
        width: parent.cellWidth - (anchors.leftMargin)
        height: parent.height / 5

        Text {
            id: text3

            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height / 2
            text: "Verticle"

            Behavior on color {
                ColorAnimation {}
            }
        }
    }

    ExclusiveGroup {
        id: postBoxRadioButt
    }

    StyledRadioButton {
        id: checkingCircle1

        group: postBoxRadioButt

        checked: true

        anchors.top: textRect1.bottom
        anchors.horizontalCenter: textRect1.horizontalCenter
        width: parent.cellWidth / 5

    }

    StyledRadioButton {
        id: checkingCircle2

        group: postBoxRadioButt

        anchors.top: textRect2.bottom
        anchors.horizontalCenter: textRect2.horizontalCenter
        width: parent.cellWidth / 5
    }

    Rectangle {
        id: input1Rect1

        color: "white"
        anchors.top: checkingCircle1.bottom
        anchors.horizontalCenter: checkingCircle1.horizontalCenter
        anchors.margins: parent.cellWidth / 7
        width: parent.cellWidth / 2; height: width * 0.8
        radius: width / 4

        border {
            color: "#666666"
        }

        TextInput {
            id: input1

            anchors.centerIn: parent
            width: parent.width / 3 * 2
            height: parent.height / 3 * 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: postingBox.varusVal
            maximumLength: 3
            font.pixelSize: height / 1.5
            KeyNavigation.tab: input2

            onTextChanged: {
                if (text == "") {
                    postingBox.varusVal = "0"
                } else {
                    postingBox.varusVal = text
                }
            }
        }
    }

    Rectangle {
        id: input1Rect2

        color: "white"
        anchors.top: textRect3.bottom
        anchors.horizontalCenter: textRect3.horizontalCenter
        anchors.margins: parent.cellWidth / 7 + checkingCircle1.height + checkingCircle1.anchors.margins
        width: parent.cellWidth / 2; height: width * 0.8
        radius: width / 4

        border {
            color: "#666666"
        }

        TextInput {
            id: input2

            anchors.centerIn: parent
            width: parent.width / 3 * 2
            height: parent.height / 3 * 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: postingBox.verticleVal
            maximumLength: 3
            font.pixelSize: height / 1.5
            KeyNavigation.tab: input1

            onTextChanged: {
                if (text == "") {
                    postingBox.verticleVal = "0"
                } else {
                    postingBox.verticleVal = text
                }
            }
        }
    }

    Rectangle {
        id: showRect1

        color: "transparent"
        anchors.top: textRect1.bottom
        anchors.horizontalCenter: textRect1.horizontalCenter
        anchors.margins: parent.cellWidth / 7 + checkingCircle1.height + checkingCircle1.anchors.margins
        width: parent.cellWidth / 2.5; height: width * 0.8
        radius: width / 4

        Text {
            color: "#B3B3B3"
            anchors.centerIn: parent
            width: parent.width / 3 * 2
            height: parent.height / 3 * 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: postingBox.varusVal + "°"
            font.pixelSize: height
        }
    }

    Rectangle {
        id: showRect2

        color: "transparent"
        anchors.top: textRect2.bottom
        anchors.horizontalCenter: textRect2.horizontalCenter
        anchors.margins: parent.cellWidth / 7 + checkingCircle1.height + checkingCircle1.anchors.margins
        width: parent.cellWidth / 2.5; height: width * 0.8
        radius: width / 4

        Text {
            color: "#B3B3B3"
            anchors.centerIn: parent
            width: parent.width / 3 * 2
            height: parent.height / 3 * 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: postingBox.verticleVal + "°"
            font.pixelSize: height
        }
    }

    Rectangle {
        id:line1

        color: "#B3B3B3"
        x: parent.cellWidth
        y: checkingCircle2.y
        height: parent.height / 2
        width: 1
    }

    Rectangle {
        id:line2

        color: "#666666"
        x: parent.cellWidth * 2
        y: checkingCircle2.y
        height: parent.height / 2
        width: 1
    }


    states: [
        State {
            name: "activeState"
            PropertyChanges { target: text1; color: "#666666"; text: "Varus" }
            PropertyChanges { target: text2; color: "#666666"; text: "Vargus" }
            PropertyChanges { target: text3; color: "#666666"}
            PropertyChanges { target: checkingCircle1; visible: true }
            PropertyChanges { target: checkingCircle2; visible: true }
            PropertyChanges { target: input1Rect1; visible: true }
            PropertyChanges { target: input1Rect2; visible: true }
            PropertyChanges { target: showRect1; visible: false }
            PropertyChanges { target: showRect2; visible: false }
            PropertyChanges { target: line1; visible: false }
            PropertyChanges { target: line2; visible: true }

        },
        State {
            name: "showingState"
            PropertyChanges {
                target: text1;
                color: "#B3B3B3";
                text: (checkingCircle1.checked) ? "Varus" : "Vargus"
            }
            PropertyChanges { target: text2; color: "#B3B3B3"; text: "Verticle" }
            PropertyChanges { target: text3; color: "transparent"}
            PropertyChanges { target: checkingCircle1; visible: false }
            PropertyChanges { target: checkingCircle2; visible: false }
            PropertyChanges { target: input1Rect1; visible: false }
            PropertyChanges { target: input1Rect2; visible: false }
            PropertyChanges { target: showRect1; visible: true }
            PropertyChanges { target: showRect2; visible: true }
            PropertyChanges { target: line1; visible: true }
            PropertyChanges { target: line2; visible: false }
        }

    ]

}


