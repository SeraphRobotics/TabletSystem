import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../../Components"
import "../SettingsPageComponents"
import ".." 1.0

// Topcoat view

SettingsPageExtensibleArea {
    id : topcoatSettings

    index : 2
    title : "Topcoat Settings"

    signal switchToModificationsPage()

    property alias currentTopcoatWeave: topcoatWeave.checkInd
    property alias currentTopcoatThicknessValue: sliderHeight.value

    property alias rec_save : rec_save

    //Value used to detect if this is topcoat settings are opened for the first time. If yes, make 'Both' button selected.
    property bool firstView : false

    function showOrHideChildren(newHeight, element)
    {
        if(newHeight > element.y)
            element.visible = true
        else
            element.visible = false
    }

    // Detect during window show or hide. We need to property hide or show all childrens.
    function heightChangedChildrenVisibleUpdate(newHeight)
    {
        showOrHideChildren(newHeight, radioButtonsArea, 0)
        showOrHideChildren(newHeight, descriptionRadioButtonsAreaText, 0)
        showOrHideChildren(newHeight, descriptionTopcoatWeave, 0)
        showOrHideChildren(newHeight, sliderTextDescription, sliderTextDescription.height)
        showOrHideChildren(newHeight, sliderHeight, sliderHeight.height)
        showOrHideChildren(newHeight, sliderTools, sliderTools.height)
        showOrHideChildren(newHeight, topcoatWeave, 0)
        showOrHideChildren(newHeight, buttonFirstArea, 0)
        rec_save.isSave = true
    }

    function resetTopcoatValues()
    {
        topcoatWeave.checkInd = topcoatBindig.calculateValue()
        sliderHeight.value = sliderBinding.calculateValue()
    }


    function checkIfValuesTheSameAndCurrentSettingsAlso()
    {
        var current

        if(settingsPageManager.currentSelectedDirection === "left")
            current = SettingsPageComponentsSettings.topcoatThicknessLeft ===
                    currentTopcoatThicknessValue &&
                    SettingsPageComponentsSettings.topcoatWeaveLeft ===
                    currentTopcoatWeave
        else if(settingsPageManager.currentSelectedDirection === "right")
            current = SettingsPageComponentsSettings.topcoatThicknessRight ===
                    currentTopcoatThicknessValue &&
                    SettingsPageComponentsSettings.topcoatWeaveRight ===
                    currentTopcoatWeave
        else if(SettingsPageComponentsSettings.checkIfValuesTheSame() === true
                &&  SettingsPageComponentsSettings.topcoatThicknessRight ===
                currentTopcoatThicknessValue &&
                SettingsPageComponentsSettings.topcoatWeaveRight ===
                currentTopcoatWeave)
            current = true


        return (current)
    }


    Item {
        id : mainWindow

        height : parent.height
        width : parent.width

        state : "noTopcoat"

        ExclusiveGroup {
            id: topcoatSettingRadioButton
        }

        Row {
            id : buttonFirstArea

            opacity : 1

            Binding {
                target: buttonFirstArea
                property: "opacity"
                value:   (mainWindow.state === "autoTopcoat") === true ? true : false
            }

            spacing : 10
            anchors {
                top : descriptionRadioButtonsAreaText.top
                topMargin : 0
                right : parent.right
                rightMargin: 40
            }

            Rectangle{
                width: 58
                height: 58

                ImageBasedButton {
                    source : "qrc:/QmlResources/cancel.png"
                    anchors.centerIn: parent
                    onCustomClicked: {
                        switchToModificationsPage()
                    }
                }
            }

            // Blue border if values saved.
            Rectangle{
                id: rec_save

                // Value used to show that all settings were saved.
                property bool isSave: true
                width: 58
                height: 58
                radius: 8
                color: isSave? "#2ECCFA" : "transparent"

                ImageBasedButton {
                    id : acceptButton

                    anchors.centerIn: parent
                    source : "qrc:/QmlResources/accept.png"

                    onCustomClicked:
                    {
                        SettingsPageComponentsSettings.setTopcoatThickness(sliderHeight.value,
                                                                           false,
                                                                           topcoatWeave.checkInd)
                        rec_save.isSave = true

                    }

                }
            }
        }

        GrayDescriptionText
        {
            id : descriptionRadioButtonsAreaText

            clip : true

            anchors {
                top : parent.top
                topMargin: 80
                left : parent.left
                leftMargin: 30
            }

            font.pixelSize: 22
            text : "Topcoat Options"
        }

        Column {
            id : radioButtonsArea

            spacing : 3

            clip : true

            anchors {
                left : descriptionRadioButtonsAreaText.left
                top : descriptionRadioButtonsAreaText.bottom
                topMargin: 20
                right : parent.right
            }
            StyledRadioButton {
                id : autoTopcoat

                group: topcoatSettingRadioButton

                descriptionText : "Auto-Topcoat (settings below)"

                onAreaClicked: {
                    mainWindow.state = "autoTopcoat"
                }
            }
            StyledRadioButton {
                id : noTopcoat

                group: topcoatSettingRadioButton
                checked: true

                descriptionText : "No Topcoat"

                onAreaClicked: {
                    mainWindow.state = "noTopcoat"
                }
            }
            StyledRadioButton {
                id : clothTopcoat

                group: topcoatSettingRadioButton

                descriptionText : "Lay glue for Cloth Topcoat"

                onAreaClicked: {
                    mainWindow.state = "clothTopcoat"
                }
            }
        }

        GrayDescriptionText {
            id : sliderTextDescription

            clip : true

            anchors {
                top : radioButtonsArea.bottom
                topMargin: 50
                left : sliderHeight.left
            }

            font.pixelSize: 22
            text : "Topcoat Thickness"

            Behavior on opacity {PropertyAnimation {}}
        }

        StyledSlider {
            id : sliderHeight

            anchors {
                top : sliderTextDescription.bottom
                topMargin: 20
                left : topcoatWeave.left
            }
            width : topcoatWeave.width+150

            Binding {
                id : sliderBinding

                target: sliderHeight
                property: "value"
                value:   calculateValue()

                // Function used to calculate current slider value.
                function calculateValue()
                {
                    if(settingsPageManager.currentSelectedDirection === "left")
                        return SettingsPageComponentsSettings.topcoatThicknessLeft
                    else if(settingsPageManager.currentSelectedDirection === "right")
                        return SettingsPageComponentsSettings.topcoatThicknessRight
                    else if(settingsPageManager.currentSelectedDirection === "both")
                        return SettingsPageComponentsSettings.topcoatThicknessLeft ===
                                SettingsPageComponentsSettings.topcoatThicknessRight ?
                                    SettingsPageComponentsSettings.topcoatThicknessLeft : 6
                }
            }

            slider.onValueChanged: {
                if(mmValues != null)
                    mmValues.text = value
            }
            Behavior on opacity {PropertyAnimation {}}
        }

        Column {
            id : sliderTools

            anchors {
                top : sliderHeight.top
                topMargin: -60
                left : sliderHeight.right
                leftMargin: 20
            }

            DefaultValueIcon {
            }
            spacing : 20

            TextFieldWithUnit {
                id : mmValues

                height : 30
                width : 60

                slider : sliderHeight
            }
            TextField {
                id : psiValue

                readOnly: true
                text : "50 PSI"
                height : 30
                width : 60
            }

            Behavior on opacity { PropertyAnimation { } }
        }

        GrayDescriptionText {
            id : descriptionTopcoatWeave

            anchors {
                top : sliderTools.bottom
                topMargin: 40
                left : parent.left
                leftMargin: 30
            }

            font.pixelSize: 22
            text : "Topcoat Weave"

            Behavior on opacity { PropertyAnimation { } }
        }

        Row {
            id : topcoatWeave

            spacing : 35
            clip : true

            anchors {
                top : descriptionTopcoatWeave.bottom
                topMargin: 25
                left : descriptionTopcoatWeave.left
                leftMargin: 5
            }

            // Value used to store selected item index.
            property int checkInd: -1

            Binding {
                id : topcoatBindig

                target: topcoatWeave
                property: "checkInd"
                value:   calculateValue()

                // Function used to calculate current slider value.
                function calculateValue()
                {
                    if(settingsPageManager.currentSelectedDirection === "left")
                        return SettingsPageComponentsSettings.topcoatWeaveLeft
                    else if(settingsPageManager.currentSelectedDirection === "right")
                        return  SettingsPageComponentsSettings.topcoatWeaveRight
                    else if(settingsPageManager.currentSelectedDirection === "both")
                        return SettingsPageComponentsSettings.topcoatWeaveLeft ===
                                SettingsPageComponentsSettings.topcoatWeaveRight ?
                                    SettingsPageComponentsSettings.topcoatWeaveLeft : 1
                }
            }

            Repeater {
                id: topCoatWaveRepeater

                clip : true
                model : ["qrc:/exampleImages/topcoat-1.png",
                    "qrc:/exampleImages/topcoat-2.png",
                    "qrc:/exampleImages/topcoat-3.png"]
                Image {
                    clip : true
                    source : modelData

                    Rectangle {
                        property bool checked: (topcoatWeave.checkInd == index) ? true : false

                        anchors.fill: parent
                        color: "transparent"

                        radius: width / 4.5

                        border.color: (checked == true) ? "#588EE6" : "transparent"
                        border.width: 2

                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                topcoatWeave.checkInd = index
                            }
                        }

                        Behavior on border.color {
                            ColorAnimation { }
                        }
                    }
                }
            }
            Behavior on opacity { PropertyAnimation { } }
        }

        states : [
            State {
                name : "autoTopcoat"

                PropertyChanges { target:  sliderTools; opacity : 1; }
                PropertyChanges { target:  topcoatWeave; opacity : 1; }
                PropertyChanges { target:  sliderTextDescription; opacity :1 }
                PropertyChanges { target:  descriptionTopcoatWeave; opacity : 1; }
                PropertyChanges { target:  sliderHeight; opacity : 1; }
                PropertyChanges { target: settingsPageManager; currentSelectedDirection:
                    {
                        if(topcoatSettings.firstView == false)
                        {
                            topcoatSettings.firstView = true;
                            return "both" ;
                        }

                        else
                        {
                            return settingsPageManager.currentSelectedDirection
                        }
                    }
                }

            },
            State {
                name : "noTopcoat"

                PropertyChanges { target:  sliderTools; opacity : 0; }
                PropertyChanges { target:  topcoatWeave; opacity : 0; }
                PropertyChanges { target:  sliderTextDescription; opacity : 0; }
                PropertyChanges { target:  descriptionTopcoatWeave; opacity : 0; }
                PropertyChanges { target:  sliderHeight; opacity : 0; }
            },
            State {
                name : "clothTopcoat"

                PropertyChanges { target:  sliderTools; opacity : 0; }
                PropertyChanges { target:  topcoatWeave; opacity : 0; }
                PropertyChanges { target:  sliderTextDescription; opacity :0 }
                PropertyChanges { target:  descriptionTopcoatWeave; opacity : 0; }
                PropertyChanges { target:  sliderHeight; opacity : 0; }
            }
        ]
    }

    onCurrentTopcoatThicknessValueChanged:
    {
        if( checkIfValuesTheSameAndCurrentSettingsAlso() === true)
        {
            rec_save.isSave = true
        }
        else
            rec_save.isSave = false
    }

    onCurrentTopcoatWeaveChanged:
    {
        if( checkIfValuesTheSameAndCurrentSettingsAlso() === true)

            rec_save.isSave = true
        else
            rec_save.isSave = false
    }


    onHeightChanged:
    {
        heightChangedChildrenVisibleUpdate(height)
    }
}
