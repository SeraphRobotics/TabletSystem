import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../Components"
import "../../Components/View3d"
import "SettingsPageComponents"
import "SettingsPageComponents/DraggableElementComponents"
import "PostingSubpage"
import "ShellModificationsSubpage"
import "TopcoatSettingsSubpage"

import "../ChoosePatientScreenPrivateComponents" 1.0
import "." 1.0

// Screen for showing All Settings page - logic from page 20-30 from pdf.
Rectangle {
    id : mainPage
    
    color : "transparent"

    property string earlierState

    property alias  sideNavigationElement: sideNavigationElement
    property alias  sideNavigation : sideNavigation
    property alias  shellModifications : shellModifications
    anchors
    {
        top : parent.top
        topMargin: 45
        left : parent.left
        right: parent.right
        bottom : parent.bottom
    }

    // Function used to restore a basic state of the view with 'ShellModifications', 'Topcoat Settings', 'Posting' settings.
    function hideAllHeaders()
    {
        topcoatSettings.state = "up"
        if(settingsPageManager.shellModificationsState != "up")
            settingsPageManager.shellModificationsState = "down"
        posting.state = "up"
    }

    UnselectAreaForDraggableElements
    {
    }

    // Item used to recalculate positions of pad labels after resizing the app.
    Timer
    {
        id : setLabelsTimer

        interval: 0
        onTriggered:
        {
            SettingsPageComponentsSettings.setLabels()
        }
    }
    
    // Item used to position the custom qml items used to show 3D graphics
    Rectangle {
        id: sideNavigation

        color : "transparent"

        z : shellModifications.z+99

        anchors {
            left : extensibleAreas.right
            right : parent.right
            top: parent.top
            topMargin:  0
            bottom: parent.bottom
        }

        // Item used to show stl file and all pads within.
        // We need to keep aspect ratio here Width/Height = 0.533.
        SettingsPageChooseSideComponent
        {
            id: sideNavigationElement

            Binding
            {
                target: sideNavigationElement
                property: "state"
                value: settingsPageManager.currentSelectedDirection
            }
            
            anchors
            {
                horizontalCenter: sideNavigation.horizontalCenter
                horizontalCenterOffset: 0
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: -20
            }

            leftSide
            {
                //Keep the aspect ratio of an element according to width and height while resize parent.
                //Width/Height = 0.533
                width: Math.min(0.76*0.533*parent.width, 0.76*0.533*parent.height)
                height: Math.min((0.76*parent.height), (0.76*parent.width))
                view3dSource: "qrc:/exampleImages/left.stl"

                rawImageSource: "qrc:/exampleImages/left-scan-cut.png"
            }
            
            rightSide
            {
                //Keep the aspect ratio of an element according to width and height while resize parent.
                //Width/Height = 0.533
                width:Math.min(0.76*0.533*parent.width, 0.76*0.533*parent.height)
                height: Math.min((0.76*parent.height), (0.76*parent.width))
                view3dSource: "qrc:/exampleImages/right.stl"
                rawImageSource: "qrc:/exampleImages/right-scan-cut.png"
            }

            Component.onCompleted:
            {
                // Setting pointers objects that is our mediator.
                SettingsPageComponentsSettings.m_LeftSideComponent = leftSide
                SettingsPageComponentsSettings.m_RightSideComponent = rightSide
                SettingsPageComponentsSettings.m_CurrentSelectedArea = rightSide
            }

            onStateChanged:
            {
                if(state === "left")
                    SettingsPageComponentsSettings.m_CurrentSelectedArea = leftSide
                else if(state === "right")
                    SettingsPageComponentsSettings.m_CurrentSelectedArea = rightSide
            }

            // When clicked 'Both' button and settingsPageManager.modificationState === "topcoat"
            // check if settings changed and show GrayPopup if yes.
            onShowTopcoatApplySettingsPopup:
            {
                var setValuesDiffrent

                if(sideNavigationElement.state === "left")
                    setValuesDiffrent = (topcoatSettings.currentTopcoatThicknessValue
                                         === SettingsPageComponentsSettings.topcoatThicknessLeft &&
                                         topcoatSettings.currentTopcoatWeave ===
                                         SettingsPageComponentsSettings.topcoatWeaveLeft)
                else if(sideNavigationElement.state === "right")
                    setValuesDiffrent = (topcoatSettings.currentTopcoatThicknessValue
                                         === SettingsPageComponentsSettings.topcoatThicknessRight &&
                                         topcoatSettings.currentTopcoatWeave ===
                                         SettingsPageComponentsSettings.topcoatWeaveRight)

                if(SettingsPageComponentsSettings.checkIfValuesTheSame() === true
                        && setValuesDiffrent === true)
                {
                    settingsPageManager.currentSelectedDirection = "both"
                    return
                }

                GrayPopup.setStateAndOpacity("topcoatSettingsApply", 1)
            }

            Connections {
                target: GrayPopup

                //This signal is emmited when user press "yes" button.
                onFirstButtonClickedSignal:
                {
                    SettingsPageComponentsSettings.setTopcoatThickness(
                                topcoatSettings.currentTopcoatThicknessValue,
                                true,
                                topcoatSettings.currentTopcoatWeave)
                    settingsPageManager.currentSelectedDirection = "both"

                    // Added blue border to "accept button"
                    if( topcoatSettings.checkIfValuesTheSameAndCurrentSettingsAlso() === true)
                    {
                        topcoatSettings.rec_save.isSave = true
                    }
                    else
                        topcoatSettings.rec_save.isSave = false
                }
            }
        }
        
        /// Posting header 3d views.
        View3dPosting {
            id: view3dPosting
            
            anchors {
                top: parent.top
                bottom : parent.bottom
                topMargin: 30
                bottomMargin: 30
                
                left : parent.left
                leftMargin: 80
                right : parent.right
                rightMargin: 80
            }
            // basic values.
            meshSource: SettingsPageComponentsSettings.m_StlFilePath0
            meshColor: "lightgrey"
            commonNavigation: false
            commonFillColor: "transparent"
            opacity : 0
        }
    }
    
    Text {
        id: showHeightMapLabel
        
        color: "#ed1c24"
        
        font.pixelSize: 22
        
        state: "HeightMapNotVisible"
        
        anchors {
            horizontalCenter: sideNavigation.horizontalCenter
            bottom : sideNavigation.bottom
            bottomMargin: 20
        }
        
        MouseArea {
            anchors.fill: parent
            onClicked: {
                showHeightMapLabel.state = (showHeightMapLabel.state === "HeightMapVisible")
                        ? "HeightMapNotVisible"
                        : "HeightMapVisible"
            }
        }
        
        states: [
            State {
                name: "HeightMapVisible"
                PropertyChanges { target: showHeightMapLabel; text: "hide height map" }
                PropertyChanges { target: sideNavigationElement.leftSide; rawImageVisible: true }
                PropertyChanges { target: sideNavigationElement.rightSide; rawImageVisible: true }
            },
            State {
                name: "HeightMapNotVisible"
                PropertyChanges { target: showHeightMapLabel; text: "show height map" }
                PropertyChanges { target: sideNavigationElement.leftSide; rawImageVisible: false }
                PropertyChanges { target: sideNavigationElement.rightSide; rawImageVisible: false }
            }
        ]
    }
    
    // Left panel to show all headers.
    Column {
        id : extensibleAreas
        
        spacing : extensibleAreaSpaceBeetwen

        property int itemWidth : ((mainPage.width-20)/2)-80

        anchors {
            top : parent.top
            left : parent.left
            leftMargin: 20
        }
        
        TopcoatSettings {
            id : topcoatSettings

            width : extensibleAreas.itemWidth
            
            onHeaderPressed: {
                console.log("topcat")
                mainPage.state = "topcoat"
            }

            onSwitchToModificationsPage: {
                shellModifications.switchFromTopcoat()
                mainPage.state = "shellModifications"
            }
            
        }
        ShellModifications {
            id : shellModifications

            objectName: "shellModifications"

            width : extensibleAreas.itemWidth

            function switchFromTopcoat()
            {
                shellModifications.showDurationMs = 20
            }

            onHeaderPressed: {
                if(topcoatSettings.state == "down" )
                    switchFromTopcoat()
                else
                    shellModifications.showDurationMs = 600
                
                mainPage.state = "shellModifications"
            }
            
            onEraseCustomElement: {
                SettingsPageComponentsSettings.m_CurrentSelectedArea.clearDrawArea(type)
            }
            onSaveCustomElement: {
                SettingsPageComponentsSettings.m_CurrentSelectedArea.customizePadName = padName
                GrayPopup.setStateAndOpacity("padCreatorPopup", 1)
            }
            onRedoPaintEvent: {
                console.log("Redo event")
                SettingsPageComponentsSettings.m_CurrentSelectedArea.cRedo()
            }
            onUndoPaintEvent: {
                console.log("Undo event")
                SettingsPageComponentsSettings.m_CurrentSelectedArea.cUndo()
            }
        }

        Posting {
            id : posting
            
            showDurationMs : 1

            width : extensibleAreas.itemWidth
            
            onHeaderPressed: {
                mainPage.state = "posting"
            }
        }
    }
    
    // Catch gray save to tollbar popup if user customized or created pad
    // and then clicked accepted button .Can choose two option ,both is
    // catch here.
    Connections {
        target : stateManager
        
        onSigSaveToToolbar:
        {
            // Add pad to pad list.
            SettingsPageComponentsSettings.m_CurrentSelectedArea.saveDrawAreaToToolbar()
            settingsPageManager.shellModificationsState = "down"
        }
        onSigUseOnlyForThisPatient:
        {
            SettingsPageComponentsSettings.m_CurrentSelectedArea.useOnlyForThisPad()
            settingsPageManager.shellModificationsState = "down"
        }
    }
    
    states: [
        State {
            name: "topcoat"
            PropertyChanges { target: view3dPosting; opacity: 0 }
            PropertyChanges { target: sideNavigationElement; opacity: 1 }
            PropertyChanges { target: showHeightMapLabel; visible: true }
            StateChangeScript {
                script : hideAllHeaders()
            }
            PropertyChanges { target: topcoatSettings; state: "down" }

            StateChangeScript {
                script : SettingsPageComponentsSettings.hideDraggableElements()
            }
            StateChangeScript {
                script : topcoatSettings.resetTopcoatValues()
            }
        },
        State {
            name: "posting"
            PropertyChanges { target: view3dPosting; opacity: 1 }
            PropertyChanges { target: sideNavigationElement; opacity: 0 }
            PropertyChanges { target: showHeightMapLabel; visible: false }
            StateChangeScript {
                script : hideAllHeaders()
            }
            PropertyChanges { target: posting; state: "down" }
            StateChangeScript {
                script : SettingsPageComponentsSettings.hideDraggableElements()
            }
        },
        State {
            name: "shellModifications"
            PropertyChanges { target: view3dPosting; opacity: 0 }
            PropertyChanges { target: sideNavigationElement; opacity: 1 }
            PropertyChanges { target: showHeightMapLabel; visible: true }
            StateChangeScript {
                script : hideAllHeaders()
            }
            PropertyChanges { target: settingsPageManager; shellModificationsState: "down" }
            StateChangeScript {
                script : SettingsPageComponentsSettings.showDraggableElements()
            }
        },
        State {
            name: "3dReview"

            PropertyChanges { target: selectPage;  opacity : 0 }
            PropertyChanges { target: modificationPage; opacity : 1 }
            PropertyChanges { target: viewButton;  visible : true }
            PropertyChanges { target: topNavigationPanel.leftButton;
                buttonText : "3: settings"}
            PropertyChanges { target: topNavigationPanel.middleButton;
                buttonText : "step 4: review"}
            PropertyChanges { target: topNavigationPanel.rightButton;
                buttonText : "5: save & load to USB"}
            PropertyChanges { target: sideNavigationElement;
                opacity: 1 }
            AnchorChanges {
                target: sideNavigation
                anchors.horizontalCenter: undefined
                anchors.left:  sideNavigation.parent.left
            }
            PropertyChanges { target: view3dPosting; opacity: 0 }
            PropertyChanges { target: extensibleAreas; visible: false }
            PropertyChanges { target: sideNavigation; anchors.topMargin:  60 }
            PropertyChanges { target: showHeightMapLabel; visible: true }
            PropertyChanges { target: sideNavigationElement; spacing: 120 }
            PropertyChanges { target: sideNavigationElement; anchors.verticalCenterOffset: -46 }

            PropertyChanges { target: sideNavigationElement.leftSide
                width:  Math.min((0.97*sideNavigationElement.height)*0.533,
                                 (0.97*sideNavigationElement.width)*0.533) }
            PropertyChanges { target: sideNavigationElement.leftSide
                height: Math.min(0.97*sideNavigationElement.height,
                                 0.97*sideNavigationElement.width )}
            PropertyChanges { target: sideNavigationElement.rightSide
                width:  Math.min((0.97*sideNavigationElement.height)*0.533,
                                 (0.97*sideNavigationElement.width)*0.533)}
            PropertyChanges { target: sideNavigationElement.rightSide
                height: Math.min(0.97*sideNavigationElement.height,
                                 0.97*sideNavigationElement.width )}

            StateChangeScript {
                script : SettingsPageComponentsSettings.showDraggableElements()
            }
            PropertyChanges { target: setLabelsTimer; running: true}
        }
    ]
    onStateChanged: {
        settingsPageManager.modificationState = state
        if(state === "")
            return
        
        if(state != "3dReview")
            earlierState = state


        SettingsPageComponentsSettings.unselectCurrentDraggablePad()
    }

    // Catch resize window and set labels according new window geometry.
    onWidthChanged:
    {
        setLabelsTimer.running = true
    }
    onHeightChanged:
    {
        setLabelsTimer.running = true
    }

    // On the begginging shellModifications area is down as is on pdf.
    Component.onCompleted:
    {
        state = "shellModifications"
    }

}
