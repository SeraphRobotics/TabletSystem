import QtQuick 2.4

import "PrivateComponents"
import "../Components"
import "../RegularScreens" 1.0
import "../"

PageTemplate {
    id : rootObject

    NavigationButton {
        id : leftNavigationButton

        buttonText : "back & undo changes"

        onButtonClicked:  {
            stateManager.setState(stateManager.previousState)
        }
        Component.onCompleted: {
            leftOrRightButton("left")
        }
    }

    NavigationButton {
        id : rightNavigationButton

        buttonText : "save & exit"
        onButtonClicked:  {
            applicationSettings.
            saveAccountSettingsToIniFile(accountSettingsElement.pinRequireValue,
                                         accountSettingsElement.loginByPrescriberValue,
                                         accountSettingsElement.autoAssignPatientsValue)

            if (stateManager.previousState !== "saveSettings") {
                stateManager.setState(stateManager.previousState)
            } else {
                stateManager.setState("startScreenState")
            }
        }
        Component.onCompleted: {
            leftOrRightButton("right")
        }
    }

    AccountSettingsGroupBox {
        id : accountSettingsElement

        pinRequireValue : applicationSettings.pinRequire
        loginByPrescriberValue : applicationSettings.loginByPrescriber
        autoAssignPatientsValue: applicationSettings.autoAssignPatients

        property real widthScaleValue: 510/1280
        property real heightScaleValue: 220/800

        height: heightScaleValue*parent.height
        width:  widthScaleValue*parent.width

        anchors{
            right : parent.right
            rightMargin: 100
            top : parent.top
            topMargin: 80
        }
    }

    PrescriberAccounts {
        id : prescriberAccounts

        anchors {
            left : parent.left
            leftMargin: 80
            top : parent.top
            topMargin: 80
        }
    }

    EditAccountSetupPage {
        id : editAccountSetupPage

        visible : false

        function setSiblingsVisible(value)
        {
            buttonFrame.visible = value
            prescriberAccounts.visible = value
            accountSettingsElement.visible = value
            textsBox.visible = value
            editAccounts.visible = value
            currentlyLoggedUser.visible = value
            buttonsPlace.visible = value
        }

        onVisibleChanged: {
            setSiblingsVisible(!visible)
        }
    }

    StyledButton {
        id : editAccounts

        width : 220
        titleText : "edit/delete accounts"

        anchors {
            right: prescriberAccounts.right
            rightMargin: 20
            top : prescriberAccounts.bottom
            topMargin: 20
        }

        onCustomClicked: {
            editAccountSetupPage.visible = true
        }
    }

    CurrentlyLogedUserInfoText {
        id : currentlyLoggedUser

        anchors {
            left : prescriberAccounts.left
            top : editAccounts.bottom
            topMargin: 40/800*parent.height
            right : prescriberAccounts.right
        }
    }

    Column {
        id : buttonsPlace

        spacing : 15
        anchors {
            top : currentlyLoggedUser.bottom
            topMargin: 15
            horizontalCenter: currentlyLoggedUser.horizontalCenter
        }

        StyledButton {
            id : switchUser

            opacity : 0.3

            width : 150
            titleText : "switch user"
        }
        StyledButton {
            id : logout

            width : 150
            titleText : "logout"

            onCustomClicked: {
                stateManager.setState("startScreenState")
            }
        }
    }

    Rectangle {
        id : buttonFrame

        border {
            width : 2
            color: "white"
        }

        radius : 10
        smooth : true

        color : "transparent"

        anchors {
            top : currentlyLoggedUser.top
            topMargin: 10
            bottom : currentlyLoggedUser.bottom
            bottomMargin: 10
            left : accountSettingsElement.left
            leftMargin: 80
            right : accountSettingsElement.right
            rightMargin: 65
        }

        ImageBasedButton {
            id : videoTutorials

            source : "qrc:/QmlResources/camera.png"
            anchors {
                verticalCenter: parent.verticalCenter
                left : parent.left
                leftMargin: 30
            }

            onCustomClicked : {
                FramePopup.showPopup()
            }
        }

        ImageBasedButton {
            id : patientDemo

            source : "qrc:/QmlResources/patient.png"
            anchors {
                verticalCenter: parent.verticalCenter
                right : parent.right
                rightMargin: 30
            }
            onCustomClicked : {
                FramePopup.showPopup()
            }
        }
    }

    Rectangle {
        id : textsBox

        height : 90

        radius : 10
        smooth: true
        color : "white"

        anchors {
            left : currentlyLoggedUser.left
            leftMargin: 20
            right : accountSettingsElement.right
            rightMargin: 80
            bottom : parent.bottom
            bottomMargin: 20
        }

        Column {
            id : firstColumn

            spacing : 5
            anchors {
                left : parent.left
                leftMargin: 15
                top : parent.top
                topMargin: 15
            }

            GrayDescriptionText {
                font {
                    pixelSize: 20
                }
                text : "contact for support"
                mouseArea.cursorShape: Qt.PointingHandCursor

                onLinkClicked:  {
                    qmlCppWrapper.iFrameUrl = applicationSettings.contactSupportUrl()
                    FramePopup.showPopup()
                }

            }
            GrayDescriptionText {
                font {
                    pixelSize: 20
                }
                text : "order materials"
                mouseArea.cursorShape: Qt.PointingHandCursor
                onLinkClicked:  {
                    qmlCppWrapper.iFrameUrl = applicationSettings.orderMaterialsUrl()
                    FramePopup.showPopup()

                }
            }
        }
        Column {
            id : secondColumn

            spacing : 5

            anchors {
                left : firstColumn.right
                leftMargin: 15
                top : parent.top
                topMargin: 15
            }

            GrayDescriptionText {
                font {
                    pixelSize: 20
                }
                text : "backup patients"
                mouseArea.cursorShape: Qt.PointingHandCursor
            }
            GrayDescriptionText {
                font {
                    pixelSize: 20
                }
                text : "usage tracker"
                mouseArea.cursorShape: Qt.PointingHandCursor
            }
        }

        GrayDescriptionText {
            anchors {
                right : parent.right
                rightMargin: 15
                top : parent.top
                topMargin: 10
            }

            font {
                pixelSize: 15
            }
            text : "customer support<br/>
                    info@seraphrobotics.com<br/>
                    555-555-555<br/>"
        }
    }
}
