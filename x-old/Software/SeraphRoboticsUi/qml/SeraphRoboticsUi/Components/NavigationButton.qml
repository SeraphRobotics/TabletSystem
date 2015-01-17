import QtQuick 2.4

Rectangle {
    id : button

    width: buttonInformationText.width+arrowImage.sourceSize.width+30
    height: buttonInformationText.height+20

    signal buttonClicked()

    color : "transparent"

    property string buttonAlignmentType : "right"

    property alias buttonInformationText: buttonInformationText
    property alias buttonText: buttonInformationText.text

    function leftOrRightButton( type) {
        buttonAlignmentType = type

        if( buttonAlignmentType === "center") {
            arrowImage.source = "qrc:/QmlResources/tutorial.png"
            arrowImage.anchors.right = button.right
            arrowImage.anchors.rightMargin = 10
            buttonInformationText.anchors.right = arrowImage.left
            buttonInformationText.anchors.rightMargin = 10
        }
        if( buttonAlignmentType === "right") {
            anchors.right = parent.right
            anchors.rightMargin = 70
            anchors.top = parent.top
            anchors.topMargin= 5
            arrowImage.source = "qrc:/QmlResources/arrow-right.png"
            arrowImage.anchors.right = button.right
            arrowImage.anchors.rightMargin = 10
            buttonInformationText.anchors.right = arrowImage.left
            buttonInformationText.anchors.rightMargin = 10
        }
        if(buttonAlignmentType === "left" ) {
            anchors.left =parent.left
            anchors.leftMargin= 70
            anchors.top = parent.top
            anchors.topMargin= 5
            arrowImage.source = "qrc:/QmlResources/arrow-left.png"
            arrowImage.anchors.left = button.left
            arrowImage.anchors.leftMargin = 10
            buttonInformationText.anchors.left = arrowImage.right
            buttonInformationText.anchors.leftMargin = 10
        }
    }

    Image {
        id : arrowImage

        smooth : true

        anchors {
           verticalCenter: parent.verticalCenter
        }
    }

    Text {
        id : buttonInformationText

        color: "#666666"
        font {
            pixelSize: 20
        }

        anchors {
            verticalCenter: parent.verticalCenter
        }

        text : "continue"
    }

    MouseArea {
        anchors.fill: (buttonAlignmentType === "center") ? arrowImage
                                                         : parent

        cursorShape: Qt.PointingHandCursor

        onClicked: {
            buttonClicked()
        }
    }
}
