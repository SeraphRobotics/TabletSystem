import QtQuick 2.4

import "../../../Components"
import "../SettingsPageComponents/DraggableElementComponents"
import "../" 1.0

Rectangle {
    id : _delegate

    width : widthElement.width/3
    height : 170/540*draggableList.height

    color : "transparent"

    property Item widthElement :  null
    property Item draggableList :  null

    Rectangle {
        id : elementWithBorder

        width : parent.width-margin
        height : parent.height-margin

        property int margin :  15

        anchors {
            left : parent.left
            leftMargin: margin/2
            top :parent.top
            topMargin: margin/2
        }

        radius : 10

        gradient : name === "Create Custom Pad" ? customPadCreatorGradient : normalGradient

        border {
            color : "#2e3192"
            width : 1
        }

        Gradient {
            id : customPadCreatorGradient

            GradientStop { position: 0.0; color: "#4b4b9b" }
            GradientStop { position: 0.5; color: "#4b4b9b" }
            GradientStop { position: 0.51; color: "#24265a" }
            GradientStop { position: 0.75; color: "#292d65" }
            GradientStop { position: 1.0; color: "#4757a1" }
        }

        Gradient {
            id : normalGradient

            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 1.0; color: "transparent" }
        }

        GrayDescriptionText {

            font.pixelSize: 15

            text :"<p align=\"center\">"+ name+"</p>"

            width : 80

            Component.onCompleted: {
                if(name === "Create Custom Pad")
                {
                    font.bold = true
                    color = "#ffffff"
                    width = 65
                    anchors.centerIn = parent
                }
                else
                {
                    anchors.top = parent.top
                    anchors.topMargin = 1
                    anchors.horizontalCenter = parent.horizontalCenter
                }
            }
        }

        Image {
            id : image

            source : name === "Create Custom Pad" ? "" : svgItem
            smooth : true

            anchors
            {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: 5
            }

            rotation : basicRotation

            fillMode: Image.PreserveAspectFit

            visible : name === "Create Custom Pad" ? false : true

            width : m_Width !== -1 ? 90*_delegate.width/187:
                                     sourceSize.width*_delegate.width/187
            height :m_Height !== -1 ? 90*_delegate.height/170:
                                      sourceSize.height*_delegate.height/170

        }
        MouseArea {
            id : dragArea

            anchors.fill: parent

            // Value used to track pad position while the mouse button is still pressed.
            // After the button gets released, this value will be false again.
            property bool startMove : false

            onClicked: {

                if(name === "Create Custom Pad")
                {
                    if(settingsPageManager.currentSelectedDirection === "both")
                        settingsPageManager.currentSelectedDirection = "left"

                    settingsPageManager.shellModificationsState = "padCreator"
                    padCreator.importStandardPad.state = "padCreating"

                    // Push base state into drawing history.
                    SettingsPageComponentsSettings.m_CurrentSelectedArea.cPush()
                    return
                }
            }

            onPressed: {
                // Check is the event wasn't triggered by 'import standard pad' button from ShellModificationPadCreator.qml
                if(padCreator.state === "padCreatorImportElement"
                        && name !== "Create Custom Pad")
                {
                    padCreator.importStandardPad.state = "customizePad"
                    SettingsPageComponentsSettings.m_CurrentSelectedArea.addNewPadToArea(svgItem)
                    padCreator.state = ""
                    padCreator.deleteMovablePad()
                    focus = false
                    return
                }

                else if(padCreator.state === "padCreatorImportElement"
                        || name === "Create Custom Pad")
                {
                    return;
                }


                startMove = true
                // Create selected pad.
                SettingsPageComponentsSettings.createObject(svgItem, name, "oneClicked",
                                                            undefined,
                                                            undefined,
                                                            undefined,
                                                            undefined,
                                                            basicRotation)
                focus = false
                draggableElementView.interactive = false
                mouse.accepted = true
            }

            onDoubleClicked:
            {
                var element = SettingsPageComponentsSettings.element

                // If the element was already created in 'onClicked' event handler, then delete the duplicated one.
                if(typeof (element) !== "undefined" && element !== null)
                {
                    element.destroy()
                }

                // Create selected pad.
                if(name !== "Create Custom Pad")
                {
                    SettingsPageComponentsSettings.createObject(svgItem, name, "doubleClicked",
                                                                undefined,
                                                                undefined,
                                                                undefined,
                                                                undefined,
                                                                basicRotation)
                    focus = false
                }
            }

            onMouseXChanged: {

                if(SettingsPageComponentsSettings.element === null)
                    return

                // Track pad.
                if(startMove == true) {
                    SettingsPageComponentsSettings.element.x =
                            SettingsPageComponentsSettings.element.parent.
                    mapFromItem(dragArea, mouseX,mouseY).x
                }
            }

            onMouseYChanged:  {
                if(SettingsPageComponentsSettings.element === null)
                    return

                // Track pad.
                if(startMove == true) {
                    SettingsPageComponentsSettings.element.y =
                            SettingsPageComponentsSettings.element.parent.
                    mapFromItem(dragArea, mouseX,mouseY).y
                }
            }

            onReleased: {
                if(padCreator.state === "padCreatorImportElement"
                        && name === "Create Custom Pad")
                    return

                draggableElementView.interactive = true

                if(typeof (SettingsPageComponentsSettings.element) == "undefined"
                        || SettingsPageComponentsSettings.element === null)
                    return

                // Check if pad was dropped onto the foot.
                if(startMove == true)
                {
                    SettingsPageComponentsSettings.recreateElement(mouseX, mouseY, dragArea)
                    SettingsPageComponentsSettings.element._privateElement._checkDropAreas()
                    SettingsPageComponentsSettings.element.setItemAsCurrentItem()
                    startMove = false
                }
            }
        }
    }
}
