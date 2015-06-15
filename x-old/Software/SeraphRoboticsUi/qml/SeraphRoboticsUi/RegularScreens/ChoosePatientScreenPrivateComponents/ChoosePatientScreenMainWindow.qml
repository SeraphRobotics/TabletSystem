import QtQuick 2.4

import "../../Components" 1.0
import "../../"


PageTemplate {
    id : mainContainer

    property alias leftNavigationButton: firstButton
    property alias rightNavigationButton : secondButton

    Rectangle {
        id : topNavigationPanel

        height : 30

        color : "transparent"

        anchors {
            left : parent.left
            leftMargin: 30

            right : parent.right
            rightMargin : 25

            top : parent.top
            topMargin: 10
        }

        Text {
            id : currentDoctorName

            font.pixelSize: 28

            color : "#f15a24"

            text : "Hello "+usersListModel.getSpecificItem(usersListModel.currentIndex).name.title+" "
                   +usersListModel.getSpecificItem(usersListModel.currentIndex).name.firstName+" "
                   +usersListModel.getSpecificItem(usersListModel.currentIndex).name.lastName
        }

        NavigationButton {
            id : firstButton

            anchors {
                top : parent.top
            }

            buttonInformationText.font.pixelSize: 25

            buttonText: "step 1: choose patient"

            Component.onCompleted: {
                leftOrRightButton("center")
                anchors.horizontalCenter = topNavigationPanel.horizontalCenter
                anchors.topMargin = -8
            }
            onButtonClicked: {
                TutorialWindowPopup.showPopup()
            }
        }
        NavigationButton {
            id : secondButton

            anchors {
                top : parent.top
            }

            buttonText: "2: patient history"

            Component.onCompleted: {
                leftOrRightButton("right")
                anchors.topMargin = -8
                anchors.rightMargin = 0
            }
        }
    }
}
