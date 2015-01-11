import QtQuick 2.4

import "../SettingsPageComponents"
import "../" 1.0
import "../SettingsPageComponents/DraggableElementComponents"
import "../../ChoosePatientScreenPrivateComponents" 1.0

SettingsPageExtensibleArea {
    id : shellModifications

    z : 1
    index : 1

    title : "Shell Modifications"

    property alias elementSettings : elementSettings
    property alias padCreator: padCreator
    property alias draggableElementList: repeater

    signal eraseCustomElement(string type)
    signal saveCustomElement(string padName)

    signal undoPaintEvent()
    signal redoPaintEvent()

    Binding {
        target: shellModifications
        property: "state"
        value: settingsPageManager.shellModificationsState
    }


    // Function used to leave pad create/customize view.
    function hidePadCreator(type)
    {
        SettingsPageComponentsSettings.showDraggableElements()
        GrayPopup.setStateAndOpacity("", 0)

        if(type === "startMovePad")
        {
            state = "elementSettings"
            /// @note for custom created pad
            if(SettingsPageComponentsSettings.currentDraggablePad === null)
                SettingsPageComponentsSettings.draggableObjectArray
                        [SettingsPageComponentsSettings.
                         draggableObjectArray.length-1].setItemAsCurrentItem(type)
        }
        else
        {
            _mainWindow.unlockResize()
            state = "down"
            SettingsPageComponentsSettings.unselectCurrentDraggablePad()
        }
    }

    ShellModificationPadCreator {
        id : padCreator

        anchors
        {
            top : header.bottom
            bottom: parent.bottom

            left : parent.left
            right : parent.right
        }

        visible : false
        clip : true

        onSigCancelCreateCustomElement:
        {
            eraseCustomElement(type)
        }
        onSigSaveCustomPadElement:
        {
            saveCustomElement(padName)
        }
        onSigRedo:
        {
            redoPaintEvent()
        }
        onSigUndo:
        {
            undoPaintEvent()
        }
        onSigImportStandardPad :
        {
            padCreator.state = "padCreatorImportElement"
        }
        onStopDrawingMovePad:
        {
            SettingsPageComponentsSettings.m_CurrentSelectedArea.movePadStart()
        }


        states : [
            // State used to import standard pad during custom pad creation.
            State {
                name: "padCreatorImportElement"
                extend: "down"
                PropertyChanges { target: padCreator; visible :false}
                PropertyChanges { target: dragView; visible : true }
                PropertyChanges { target: padCreator.importStandardPad;
                    state :padCreator.importStandardPad.state}
                PropertyChanges { target: dragView; z : 99}
            }
        ]
    }

    ShellModificationDraggableElementSettings {
        id : elementSettings

        visible : false

        clip : true

        anchors
        {
            top : header.bottom
            bottom: parent.bottom
            left : parent.left
            right : parent.right
        }

        onHide:
        {
            settingsPageManager.shellModificationsState = "down"
        }

        onSigCustomizePad:
        {
            settingsPageManager.shellModificationsState = "padCreator"
            padCreator.importStandardPad.state = "customizePad"
            if(settingsPageManager.currentSelectedDirection === "both")
                SettingsPageComponentsSettings.
            switchCurrentDirectionStateToCurrentUsableArea()

            SettingsPageComponentsSettings.m_CurrentSelectedArea.addPadToArea()
        }
    }

    // List of pads.
    Flickable {
        id : draggableElementView

        clip : true
        boundsBehavior: Flickable.StopAtBounds

        anchors {
            top : parent.top
            topMargin: 50
            bottom: parent.bottom
            bottomMargin: 20
            left : parent.left
            right : parent.right
        }

        contentWidth: dragView.width; contentHeight: dragView.height

        Grid {
            id : dragView

            columns: 3
            clip : true

            Repeater {
                id : repeater

                model: ShellModificationDraggableElementModel { }
                delegate: ShellModificationDraggableElementListDelegate {
                    widthElement: shellModifications
                    draggableList: draggableElementView
                }
            }
        }
    }

    states: [
        // State used to change pad basic settings.
        State {
            name: "elementSettings"
            extend: "down"
            PropertyChanges { target: elementSettings; visible :true}
            PropertyChanges { target: dragView; visible :false}
            PropertyChanges { target: padCreator; visible : false }
            PropertyChanges { target: draggableElementView; interactive : false }
        },
        // State used to customize or create a new pad.
        State {
            name: "padCreator"
            extend: "down"
            PropertyChanges { target: elementSettings; visible :false }
            PropertyChanges { target: dragView; visible :false }
            PropertyChanges { target: padCreator; visible : true }
            PropertyChanges { target: draggableElementView; interactive : false }

        },
        // State used to move pad ('stop drawing, move pad' button).
        State {
            name: "startMovePad"
            extend: "down"
            PropertyChanges { target: padCreator; visible : true }
            PropertyChanges { target: dragView; visible :false}
            PropertyChanges { target: draggableElementView; interactive : false }
            PropertyChanges { target: padCreator.importStandardPad; state: "customizePad" }
            PropertyChanges { target: elementSettings; visible : false }
        }
    ]

    transitions: [
        Transition {
            from: "elementSettings"; to: "up";
            ParallelAnimation {
                NumberAnimation { properties: "visible"; duration: 0; easing.type: Easing.Linear ; target : dragView ;to : 0}
                NumberAnimation { properties: "visible"; duration: 0; easing.type: Easing.Linear ; target : elementSettings ;to : 1}
                NumberAnimation { properties: "height"; duration: hideDurationMs; easing.type: Easing.Linear ;}
            }
        }
    ]

    onStateChanged: {
        if(state === "up")
        {
            draggableElementView.interactive = false
        }
        else if(state === "down")
        {
            padCreator.state = ""
            if(SettingsPageComponentsSettings.m_CurrentSelectedArea !== null)
                SettingsPageComponentsSettings.m_CurrentSelectedArea.clearDrawArea("clearAndClose")
            draggableElementView.interactive = true
        }
        else if(state === "padCreator")
        {
            _mainWindow.lockResize()
        }

    }

    Component.onCompleted: {
        SettingsPageComponentsSettings.m_ShellModificationsComponentPtr = shellModifications
    }
}
