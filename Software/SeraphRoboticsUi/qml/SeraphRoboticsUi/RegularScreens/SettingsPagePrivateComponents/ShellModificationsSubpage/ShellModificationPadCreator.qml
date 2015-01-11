import QtQuick 2.4

import "../../../Components"
import "../" 1.0

// Item used to customize/create pad. Remember that in normal state we drew in
// canvas Area. "Stop drawing move pad" means that we have finished drawing. In this case we leave
// canvas paint area and create pad, which now can be easily moved, resized, rotated. To return to
// normal state (paint area) we just need to click on paint or erase button.
// This changes the pad object back into canvas image.

Rectangle {
    color : "transparent"

    signal sigCancelCreateCustomElement(string type)
    signal sigSaveCustomPadElement(string padName)

    property alias drawButton: draw
    property alias eraseButton: erase
    property alias rotateButton : rotate
    property alias importStandardPad : importStandardPad
    property alias padName: padName

    // This signal is emitted when the user presses 'redo' button.
    signal sigRedo()
    // This signal is emitted when the user presses 'undo' button.
    signal sigUndo()
    signal sigImportStandardPad()
    signal stopDrawingMovePad()

    // Value used to detect if we are already in "stop drawing, move pad" state.
    property bool padCreatedAfterEscapedCanvasPaintArea: false

    // Function used to clear the "stop drawing,move pad" pad creation state.
    function deleteMovablePad()
    {
        if(padCreatedAfterEscapedCanvasPaintArea === true
                && importStandardPad.state  === "padCreating")
        {
            SettingsPageComponentsSettings.draggableObjectArray
                    [SettingsPageComponentsSettings.draggableObjectArray.length-1].destroy()
        }
        padCreatedAfterEscapedCanvasPaintArea = false
    }

    // Fuction used to check if we're in the moving or drawing state.
    // If current state is "startMovePad" then it changes to drawing state.
    function checkIfMovable()
    {
        if(settingsPageManager.shellModificationsState == "startMovePad")
        {
            // Export pad from frame to canvas area.
            SettingsPageComponentsSettings.m_CurrentSelectedArea.addPadToArea()
            settingsPageManager.shellModificationsState = "padCreator"
            // Clear pad moving history.
            SettingsPageComponentsSettings.movePadHistory.clearHistory()
            padCreatedAfterEscapedCanvasPaintArea = true
        }
    }

    function setPadName(nameValue)
    {
        padName.textFiled.text = nameValue
    }

    function getPadName()
    {
        return padName.textFiled.text
    }

    MouseArea {
        anchors.fill: parent
    }

    GrayDescriptionText {
        anchors {
            verticalCenter: buttonFirstArea.verticalCenter
            left : parent.left
            leftMargin: 25
        }

        text : "<p align=\"center\">Custom Pad<br/>
                Creator</p>"
        font.pixelSize: 27
    }

    Row {
        id : buttonFirstArea

        spacing : 10
        anchors {
            top : parent.top
            topMargin : firstLine.anchors.topMargin/2-cancelButton.height/2
            right : parent.right
            rightMargin: 50
        }

        ImageBasedButton {
            id : cancelButton

            source : "qrc:/QmlResources/cancel.png"

            onCustomClicked: {
                checkIfMovable()
                sigCancelCreateCustomElement("clearAndClose")
                deleteMovablePad()
                settingsPageManager.shellModificationsState = "down"
            }
        }

        ImageBasedButton {
            id : acceptButton

            source : "qrc:/QmlResources/accept.png"

            opacity: SettingsPageComponentsSettings.drawingAreaDetection.nothingDraw === true
                     && importStandardPad.state === "padCreating"
                     && settingsPageManager.shellModificationsState === "padCreator" ?
                         0.2   : 1

            enabled : opacity === 1

            onCustomClicked:
            {
                checkIfMovable()
                sigSaveCustomPadElement(padName.inputText)
                padCreatedAfterEscapedCanvasPaintArea = false
            }
        }
    }

    Rectangle {
        id : firstLine

        color : "#b3b3b3"

        height : 1

        anchors {
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 10
            top: parent.top
            topMargin: parent.height*0.2
        }
    }

    Item {
        id : buttonSecondArea

        anchors {
            top : firstLine.bottom
            topMargin : parent.width*0.11-draw.height/2
            left : parent.left
            leftMargin: 60
            right : buttonFirstArea.right
            bottom : secondLine.top
        }

        ImageBasedButtonWithDescription {
            id : draw

            buttonElement.source: "qrc:/QmlResources/draw.png"

            anchors {
                verticalCenter:   parent.verticalCenter
            }

            buttonElement.onCustomClicked: {
                erase.state = ""
                rotate.state = ""
                state === "" ?
                            state = "selected" : state = ""
                checkIfMovable()
            }
        }

        ImageBasedButtonWithDescription {
            id : erase

            buttonElement.source: "qrc:/QmlResources/erase.png"
            textElement.text: "erase"

            anchors {
                verticalCenter:   parent.verticalCenter
                right : parent.right
            }

            buttonElement.onCustomClicked: {
                draw.state = ""
                rotate.state = ""
                state === "" ?
                            state = "selected" : state = ""
                checkIfMovable()
            }
            buttonElement.onCustomPressed: {
                erase.state = "selected"
            }
            buttonElement.onCustomReleased:  {
                erase.state = ""
            }
        }
    }

    Rectangle {
        id : secondLine

        color : "#b3b3b3"
        height : 1
        anchors {
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 10
            top: parent.top
            topMargin: parent.height*0.50
        }
    }

    Rectangle {
        id : buttonThirdArea

        color : "transparent"
        anchors {
            top : secondLine.bottom
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 25
            bottom : thirdLine.top
        }

        StyledButton {
            id : importStandardPad

            titleText:   "<p align=\"center\">import standard pad<br/> as starting point</p>"
            text.font.pixelSize: 15
            width : 200

            anchors {
                verticalCenter:  parent.verticalCenter
            }

            text {
                textFormat: Text.RichText
            }

            onCustomClicked: {
                if(importStandardPad.opacity === 1)
                {
                    sigImportStandardPad()
                }
            }

            states: [
                State {
                    name: "customizePad"
                    PropertyChanges { target: importStandardPad; opacity: 0.2}
                },
                State {
                    name: "padCreating"
                    PropertyChanges { target: importStandardPad; opacity: 1}
                }
            ]
        }

        Row {
            id : arrowsButtonElement

            anchors {
                verticalCenter: parent.verticalCenter
                right : parent.right
                rightMargin: 25
            }

            spacing : 20

            ImageBasedButtonWithDescription {
                id : undo

                buttonElement.source:   "qrc:/QmlResources/arrow-left.png"
                textElement.text: "undo"

                opacity : SettingsPageComponentsSettings.drawingAreaDetection.nothingDraw === true
                          && SettingsPageComponentsSettings.movePadHistory.movePadHistoryCount === -1 ?
                              0.2 : 1

                anchors
                {
                    verticalCenter:   parent.verticalCenter
                }

                buttonElement.onCustomClicked:
                {
                    sigUndo()
                }

            }
            ImageBasedButtonWithDescription {
                id : redo

                buttonElement.source:   "qrc:/QmlResources/arrow-right.png"
                textElement.text: "redo"


                anchors {
                    verticalCenter:   parent.verticalCenter
                }

                buttonElement.onCustomClicked: {
                    sigRedo()
                }
            }
        }
    }

    Rectangle {
        id : thirdLine

        color : "#b3b3b3"

        height : 1

        anchors {
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 10
            top: parent.top
            topMargin: parent.height*0.70
        }
    }

    Rectangle {
        id : buttonFourthArea

        color : "transparent"

        anchors {
            top : thirdLine.bottom
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 25
            bottom : fourthLine.top
        }

        GrayDescriptionText {
            id : elementDescription

            width : 150

            anchors {
                left : parent.left
                leftMargin: 40
                verticalCenter: parent.verticalCenter
            }
            font.pixelSize: 17

            text : "<p align=\"center\">stop drawing,<br/>move pad</p>"
        }

        ImageBasedButton {
            id : rotate

            source : "qrc:/QmlResources/stop-and-draw.png"

            opacity: SettingsPageComponentsSettings.drawingAreaDetection.nothingDraw === true
            && importStandardPad.state === "padCreating"
                     && settingsPageManager.shellModificationsState === "padCreator" ?
                         0.2 : 1

            enabled : opacity === 1

            anchors
            {
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: 55
                verticalCenter: parent.verticalCenter
            }

            onCustomClicked:
            {
                if(opacity != 1)
                    return

                if(rotate.state == "selected")
                {
                    return
                }

                SettingsPageComponentsSettings.m_CurrentSelectedArea.customizePadName
                        = padName.textFiled.text
                stopDrawingMovePad()
                rotate.state = "selected"
                draw.state = ""
                erase.state = ""

            }
        }
    }

    Rectangle {
        id : fourthLine

        color : "#b3b3b3"

        height : 1

        anchors {
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 10
            top: parent.top
            topMargin: parent.height*0.90
        }
    }

    Rectangle {
        id : buttonFifthArea

        color : "transparent"

        anchors {
            top : fourthLine.bottom
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 25
            bottom : parent.bottom
        }

        StyledTextInputWText {
            id : padName

            height : 40
            width : 370

            text : "name pad "

            anchors
            {
                verticalCenter: parent.verticalCenter
            }

            // If we choose the custom pad creation, this function will fill name input
            // with random string, otherwise it does do nothing.
            function calculateName()
            {
                if(SettingsPageComponentsSettings.currentDraggablePad === null)
                {
                    textFiled.text = SettingsPageComponentsSettings.makeName()
                    return
                }

                textFiled.text = SettingsPageComponentsSettings.currentDraggablePad.m_Name
            }

            onVisibleChanged:
            {
                if(visible === true)
                {
                    calculateName()
                }
            }
        }
    }
}
