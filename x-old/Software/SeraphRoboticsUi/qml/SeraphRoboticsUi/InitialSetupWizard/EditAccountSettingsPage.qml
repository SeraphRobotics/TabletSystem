import QtQuick 2.4

import "../Components"
import "PrivateComponents"
import ".."

PageTemplate {

    active: false

    NavigationButton {
        id : leftNavigationButton

        buttonText : "back & undo changes"

        onButtonClicked:  {
            stateManager.setState("accountSetupState")
        }
        Component.onCompleted: {
            leftOrRightButton("left")
        }
    }

    NavigationButton {
        id : rightNavigationButton

        buttonText : "save & continue"
        onButtonClicked:  {
            applicationSettings.saveAccountSettingsToIniFile(accountSettings.pinRequireValue,
                                                             accountSettings.loginByPrescriberValue,
                                                             accountSettings.autoAssignPatientsValue)
            stateManager.setState("saveSettings")
        }
        Component.onCompleted: {
            leftOrRightButton("right")
        }
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

    GrayDescriptionText {
        id : topText

        opacity: 0

        anchors {
            bottom : accountSettings.top
            bottomMargin : 70
            horizontalCenter: parent.horizontalCenter
        }

        font.pixelSize: 25

        text :
            "If you choose, you may now adjust your settings."
    }

    AccountSettingsGroupBox {
        id : accountSettings

        opacity: 0

        anchors.centerIn: parent
    }

    Image {
        id : questionMark

        opacity: 0

        source : "qrc:/QmlResources/help.png"

        anchors {
            top : bottomText.top
            left : parent.left
            leftMargin: 320
        }
    }

    GrayDescriptionText {
        id : bottomText

        opacity: 0

        anchors {
            bottom : parent.bottom
            bottomMargin : 40
            left: parent.left
            leftMargin: 80
        }

        font {
            pixelSize: 25
        }

        text :
            "<b>Default Settings</b><br/>
Pin required for login<br/>
Prescribers must login as themselves<br/>
Auto-assign new patients to currently logged in prescriber<br/>
(can reassign patients later as needed)<br/>"
    }

    SequentialAnimation {
        id : fadeIn

        running: true

        NumberAnimation { target: topText; property: "opacity"; to: 1; duration: 2000 }

        ParallelAnimation {
            NumberAnimation { target: accountSettings; property: "opacity"; to: 1; duration: 2000 }
            NumberAnimation { target: bottomText; property: "opacity"; to: 1; duration: 2000 }
            NumberAnimation { target: questionMark; property: "opacity"; to: 1; duration: 2000 }
        }

        NumberAnimation { target: blueArrow; property: "opacity"; to: 1; duration: 2000 }

        onRunningChanged: {
            if(running === false)
            {
                blueArrow.startBlinking()
            }
        }
    }
}
