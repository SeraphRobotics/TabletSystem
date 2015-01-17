import QtQuick 2.4

Rectangle {

    radius : 10

    property alias title: title.text
    property alias header: header

    property int headerHeight : 45
    property int headerTextFontPixelSize : 20


    border {
        width : 1
        color : "#cccccc"
    }

    gradient:  Gradient {
        GradientStop { position: 0.0; color: "#ffffff" }
        GradientStop { position: 1.0; color: "#ebebec" }
    }

    Rectangle {
        id : middleElement

        radius : 0
        smooth : true

        height : 10
        width : parent.width-2

        z : header.z+1

        anchors {
            top : header.bottom
            left : parent.left
            leftMargin: 1
            topMargin: -5
        }

        color : "#ffffff"
    }

    Rectangle {
        id : header

        radius : 10
        width : parent.width
        height : headerHeight

        anchors {
            left : parent.left
            top : parent.top
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
                pixelSize: headerTextFontPixelSize
            }

            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -5
                horizontalCenter: parent.horizontalCenter
            }
        }
    }
}
