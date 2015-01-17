// StickyDegreelabel.qml

import QtQuick 2.0

Column {

    property int degree: 0

    anchors {
        left: parent.left
        right: parent.right
        top: parent.bottom
        bottom: parent.bottom
    }
    Rectangle {
        id: line1
        anchors {
            left: parent.left
            right: parent.right
        }
        height: 1
        border {
            width: 1
            color: "black"
        }
        color: "black"
    }
    Rectangle {
        height: parent.parent.height * 0.01
        width: 1
        color: "black"
        anchors.horizontalCenter: parent.horizontalCenter
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        font.pointSize: 6
        text: degree.toString() + "\u00B0"
        anchors.margins: 5
    }
}
