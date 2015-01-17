import QtQuick 2.4

import ".."
import "../../Components"

ListView {
    id : view

    clip : true

    width: parent.width

    highlightMoveDuration : 0

    delegate: Rectangle {
        width : parent.width-20
        height : 30

        radius: 5

        anchors {
            left : parent.left
            leftMargin: 10
        }

        color :  index === view.currentIndex  ?   "#abadd3" : "transparent"
        GrayDescriptionText {

            font.pixelSize: 20

            width : parent.width-10
            wrapMode : Text.NoWrap

            anchors {
                verticalCenter: parent.verticalCenter
                left : parent.left
                leftMargin: 10
            }

            clip : true
            elide : Text.ElideRight

            text: Qt.formatDateTime(model.dateTime, "d MMMM yyyy hh:mm:ss AP t").toString()
        }
        MouseArea {
            anchors.fill: parent

            onClicked: {
                view.currentIndex = index
            }
        }
    }
}
