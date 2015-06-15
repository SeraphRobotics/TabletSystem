import QtQuick 2.4

import ".."
import "../Components"
import "StartScreenPrivateComponents"

/*
    Main Ui screen page 12 from complete2June14.pdf
*/
PageTemplate {
    id : startScreen

    state : "buttonShowing"

    StyledButton {
        id : startButton

        z : pinInput.z + 1
        height : 80

        focus : visible
        text.color: "#ffffff"
        text.font.pixelSize: 42

        border.color: "#2e3192"

        buttonType : "startButton"
        anchors {
            top : parent.top
            topMargin: 200
            horizontalCenter: parent.horizontalCenter
        }
        onCustomClicked:
        {
            startScreen.state = "pinInputShowing"
        }
    }

    Rectangle {
        id : pinInput

        width : startButton.width
        height : startButton.height

        anchors.centerIn: startButton

        radius : 10
        visible : true
        focus : visible

        gradient : Gradient
        {
            GradientStop { position: 0.0; color: "#b6b6b6" }
            GradientStop { position: 1.0; color: "#f0f0f1"}
        }

        StyledTextInput {
            id : textPinInput

            anchors.centerIn: parent

            deafultFontSize :30
            anchors.fill: parent

            textColor: "#666666"

            focus : true
            visible : true

            welcomeTextColor:   "#e6e6e6"
            placeholderText: "enter pin"

            style : textPinInput.styleNormalInput

            Keys.onReturnPressed:
            {

                // Get the currently selected item .
                var currentItem = usersListModel.getSpecificItem(listUsers.listView.currentIndex)

                // Checks if the item exist.
                if( currentItem === null)
                {
                    return
                }

                // Corrected Password detection.
                if(textPinInput.text === currentItem.password)
                {
                    usersListModel.currentIndex = listUsers.listView.currentIndex
                    startScreen.state = "buttonShowing"
                    stateManager.setState("choosePatientState")
                }
                // Wrong password.
                else
                {
                    console.log("Password is inncorect")
                    textPinInput.style = textPinInput.styleErrorInput
                }

            }

            onTextChanged:
            {
                if (textPinInput.style == textPinInput.styleErrorInput)
                {
                    textPinInput.style = textPinInput.styleNormalInput
                }
            }
        }
    }

    StartScreenUsersList {
        id : listUsers

        anchors {
            top : startButton.bottom
            topMargin: 50
            horizontalCenter: parent.horizontalCenter
        }

        onCurrentIndexChanged:
        {
            startScreen.state = "buttonShowing"
            textPinInput.text = ""
            textPinInput.style = textPinInput.styleNormalInput
        }
    }

    StartScreenBottomPanel {
        rootVisibleElement : startScreen
        anchors {
            bottom : parent.bottom
            bottomMargin : 15
            horizontalCenter: parent.horizontalCenter
        }
    }

    states: [
        State {
            name: "buttonShowing"
            PropertyChanges { target: startButton; visible: true }
            PropertyChanges { target: pinInput; opacity: 0 }
        },
        State {
            name: "pinInputShowing"
            PropertyChanges { target: pinInput; opacity: 1 }
            PropertyChanges { target: startButton; visible: false }
        }
    ]

}
