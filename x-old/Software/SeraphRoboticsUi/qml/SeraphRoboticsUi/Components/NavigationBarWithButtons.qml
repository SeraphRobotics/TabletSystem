import QtQuick 2.4

Rectangle {
    id : topNavigationPanel

    property alias leftButton: leftButton
    property alias middleButton : middleButton
    property alias rightButton : rightButton

    signal leftButtonClicked()
    signal rightButtonClicked()
    signal middleButtonClicked()

    height : 30

    color : "transparent"

    anchors {
        left : parent.left
        leftMargin: 25

        right : parent.right
        rightMargin : 25

        top : parent.top
        topMargin: 10
    }

    NavigationButton {
        id : leftButton

        anchors {
            top : parent.top
            left : parent.left
        }

        buttonText: "2: patient history"

        Component.onCompleted: {
            leftOrRightButton("left")
            anchors.topMargin = -8
            anchors.leftMargin = 0
        }

        onButtonClicked: {
            leftButtonClicked()
        }
    }

    NavigationButton {
        id : middleButton

        anchors {
            top : parent.top
        }

        buttonInformationText.font.pixelSize: 25
        buttonText: "step 3: settings"

        Component.onCompleted: {
            leftOrRightButton("center")
            anchors.horizontalCenter = topNavigationPanel.horizontalCenter
            anchors.horizontalCenterOffset = 40
            anchors.topMargin = -8
        }
        onButtonClicked: {
            middleButtonClicked()
        }
    }
    NavigationButton {
        id : rightButton

        anchors {
            top : parent.top
        }

        buttonText: "4: review"

        onButtonClicked: {
            rightButtonClicked()
        }

        Component.onCompleted: {
            leftOrRightButton("right")
            anchors.topMargin = -8
            anchors.rightMargin = 0
        }
    }
}
