import QtQuick 2.0

Rectangle {
    id : defaultValueIcon

    width : 60
    height : 20
    color : "transparent"

    Image {
        id : icon

        source : "qrc:/QmlResources/pointer-default.png"

        anchors {
            verticalCenter: parent.verticalCenter
            left : parent.left
            leftMargin : 0
        }
    }

    GrayDescriptionText {
        text : "default"

        font.pixelSize: 13

        anchors {
            verticalCenter: parent.verticalCenter
            left : icon.right
            leftMargin: 5
        }
    }

}
