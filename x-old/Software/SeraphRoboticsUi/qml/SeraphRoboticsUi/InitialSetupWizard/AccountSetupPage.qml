import QtQuick 2.4

import ".."
import "../Components"
import "PrivateComponents"

PageTemplate {
    id :accountSetupPage

    active: false

    NavigationButton
    {
        id : leftNavigationButton

        buttonText : "back & undo changes"

        onButtonClicked:  {
            stateManager.setState("customerNumberPageState")
        }
        Component.onCompleted: {
            leftOrRightButton("left")
        }
    }

    NavigationButton {
        id : rightNavigationButton

        buttonText : "save & log on"
        onButtonClicked:  {
            stateManager.setState("editaccountSettingsPage")
        }
        Component.onCompleted: {
            leftOrRightButton("right")
        }
    }

    GrayDescriptionText {
        id : headerText

        anchors {
            top : leftNavigationButton.bottom
            horizontalCenter: parent.horizontalCenter
        }

        text : "Account Setup"
    }

    BlueArrowComponent {
        id : blueArrow

        anchors {
            top : parent.top
            topMargin: 128
            right : parent.right
            rightMargin: 214
        }
    }

    BlackDescriptionText {
        id : infoTextFirst

        opacity : (stateManager.previousState === "editaccountSetupPage")
        anchors {
            top : headerText.bottom
            topMargin: 50
            horizontalCenter: parent.horizontalCenter
        }

        text : "<p align=\"center\">You can create an account for yourself and/or<br/>
other prescribers using this machine, or<br/>
simply login as the Default user by clicking<br/>
“save & log on” without adding a new account.</p>"
    }

    BlackDescriptionText {
        id : infoTextSecond

        opacity : (stateManager.previousState === "editaccountSetupPage")
        anchors {
            top : infoTextFirst.bottom
            topMargin: 0
            horizontalCenter: parent.horizontalCenter
        }

        text : "<p align=\"center\"><br/><br/>
Accounts allow you to keep track of your<br/>
patients and personalize the software to you.</p>"
    }


    CurrentlyLogedUserInfoText {
        id : currentlyLoggedUser

        opacity : (stateManager.previousState === "editaccountSetupPage")
        anchors {
            left : parent.left
            leftMargin: 75
            top : infoTextSecond.bottom
            topMargin: 80

        }
    }

    StyledButton {
        id : switchUser

        width : 200
        titleText : "switch user"

        opacity : (stateManager.previousState === "editaccountSetupPage")

        anchors {
            horizontalCenter: currentlyLoggedUser.horizontalCenter
            top : currentlyLoggedUser.bottom
            topMargin: 20
        }

        onCustomClicked: {
            if(opacity === 1)
                stateManager.setState("editaccountSetupPage")
        }
    }

    PrescriberAccounts {
        id : prescriberAccounts

        opacity : (stateManager.previousState === "editaccountSetupPage")
        anchors {
            right : parent.right
            rightMargin: 75
            top : infoTextSecond.bottom
            topMargin: 80
        }
    }

    StyledButton {
        id : buttonsArea

        opacity : (stateManager.previousState === "editaccountSetupPage")

        width : 210
        titleText : "edit accounts"

        anchors {
            horizontalCenter: prescriberAccounts.horizontalCenter
            top : prescriberAccounts.bottom
            topMargin: 20
        }

        onCustomClicked: {
            stateManager.setState("editaccountSetupPage")
        }
    }

    SequentialAnimation {
        id : fadeIn

        running: !(stateManager.previousState === "editaccountSetupPage")

        ParallelAnimation {
            NumberAnimation { target: infoTextFirst; property: "opacity"; to: 1; duration: 3000 }
        }

        ParallelAnimation {
            NumberAnimation { target: infoTextSecond; property: "opacity"; to: 1; duration: 3000 }
        }

        ParallelAnimation {
            NumberAnimation { target: currentlyLoggedUser; property: "opacity"; to: 1; duration: 3000 }
            NumberAnimation { target: switchUser; property: "opacity"; to: 0.3; duration: 3000 }
            NumberAnimation { target: prescriberAccounts; property: "opacity"; to: 1; duration: 3000 }
            NumberAnimation { target: buttonsArea; property: "opacity"; to: 1; duration: 3000 }
        }

        NumberAnimation { target: blueArrow; property: "opacity"; to: 1; duration: 3000 }

        onRunningChanged: {
            if(running === false)
            {
                blueArrow.startBlinking()
            }
        }
    }
}
