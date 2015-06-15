import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../Components"
import ".."

Rectangle {
    width: parent.width-100
    height: textContent.height+4
    color : "transparent"

    property Item listView

    RadioButton {
        id : selectBox

        checked : listView.currentIndex === index ? true : false

        anchors {
            left : parent.left
            top : textContent.top
        }
    }

    Item {
        id : textContent

        anchors {
            top: selectBox.top
        }

        width : parent.width-60

        height : statusText.height

        GrayDescriptionText {
            id : timestampText

            font.pixelSize: 17
            text : timeStamp

            anchors {
                top : parent.top
                left : parent.left
                leftMargin: 60/560*parent.width
            }
        }

        GrayDescriptionText {
            id : typeText

            font.pixelSize: 17
            text : type

            anchors {
                left : parent.left
                leftMargin: 265/560*parent.width
            }
        }

        GrayDescriptionText {
            id : statusText

            font.pixelSize: 17
            text : status
            width : parent.width/2
            wrapMode: Text.WrapAnywhere

            anchors {
                left : parent.left
                leftMargin: 410/560*parent.width
            }
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            listView.currentIndex = index
        }

    }
}
