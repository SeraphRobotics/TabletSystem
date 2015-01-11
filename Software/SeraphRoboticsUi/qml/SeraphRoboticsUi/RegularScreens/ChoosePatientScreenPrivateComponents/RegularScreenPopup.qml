pragma Singleton

import QtQuick 2.4

import "../../Components"
Item {
    id : background

    anchors {
        fill: parent
        centerIn: parent
    }

    z : opacity === 1 ? 9999 : 0
    opacity : 0
    enabled: opacity === 1 ? true : false

    signal firstButtonClickedSignal()
    signal secondButtonClickedSignal()

    function setStateAndOpacity(state, opacityValue)
    {
        background.state = state
        background.opacity = opacityValue
    }

    MouseArea {
        anchors.fill: parent
    }


    Rectangle {
        id : popupWindow

        width: 500
        height: 260

        radius : 10
        smooth : true

        border {
            width : 1
            color: "#b3b3b3"
        }

        anchors.centerIn: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ffffff" }
            GradientStop { position:1.0; color: "#e8e9ea" }
        }

        GrayDescriptionText {
            id : descriptionText

            anchors {
                top : parent.top
                topMargin: 30
                horizontalCenter:  parent.horizontalCenter
            }

            color : "gray"
            font.pixelSize: 15

            text : "<p align=\"center\">This patient is useally treated by another doctor,<br/>
                 how would you like to proceed?</p>"
        }

        Column {
            spacing : 15

            anchors {
                horizontalCenter: parent.horizontalCenter
                top  : descriptionText.bottom
                topMargin : 30
            }

            StyledButton {
                id : firstButton

                width : 220
                height : 50
                titleText : "make my patient"

                onCustomClicked: {
                    if(background.state == "padCreatorPopup")
                    {
                        stateManager.emitSaveToToolbar()
                    }
                    else if(background.state == "topcoatSettingsApply")
                    {
                        firstButtonClickedSignal()
                    }
                    background.opacity = 0
                }
            }
            StyledButton {
                id : secondButton

                width : 220
                height : 50
                titleText : "edit only"

                onCustomClicked: {
                    if(background.state == "padCreatorPopup")
                    {
                        stateManager.emitUseOnlyForThisPatient()
                    }
                    secondButtonClickedSignal()
                    background.opacity = 0
                }
            }
        }
    }

    states : [
        State {
            name : "padCreatorPopup"
            PropertyChanges {
                target: descriptionText
                text : "<p align=\"center\">Would you like to save this pad to your tool bar<br/>
for future use on other prescriptions?</p>"
            }
            PropertyChanges {
                target: firstButton
                titleText: "Yes: save to tool bar"
            }
            PropertyChanges {
                target: secondButton
                titleText: "No: use only for this patient"
            }
            PropertyChanges {
                target: firstButton
                width: 285
            }
            PropertyChanges {
                target: secondButton
                width: 285
            }
        },
        State {
            name : "topcoatSettingsApply"
            PropertyChanges {
                target: descriptionText
                text : "<p align=\"center\">Do you want to apply<br/>
settings from current side to both sides?</p>"
            }
            PropertyChanges {
                target: firstButton
                titleText: "Yes"
            }
            PropertyChanges {
                target: secondButton
                titleText: "No"
            }
        }

    ]

}
