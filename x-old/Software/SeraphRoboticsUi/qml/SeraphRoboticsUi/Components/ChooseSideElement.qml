import QtQuick 2.4

Rectangle {

    radius : 10
    color : "transparent"

    property alias textElement: textElement
    property alias mouseArea : mouseArea

    border
    {
        width : 2
        color : "transparent"
    }

    width : textElement.width+20
    height : textElement.height+20

    GrayDescriptionText
    {
        id : textElement

        font.pixelSize: 22
        text : "left"
        anchors {
            centerIn: parent
        }
    }

    MouseArea {
        id : mouseArea

        anchors.fill: parent
    }
}
