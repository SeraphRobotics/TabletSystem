import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../../Components"
import "../" 1.0

Rectangle {
    color : "transparent"

    signal hide()
    signal sigCustomizePad()

    clip : true

    property alias elementName: elementName
    property alias imageElementPath : iconImage.source
    property alias iconImage : iconImage

    function resetSlider()
    {
        sliderContainer.leftTriangle.x = 80
        sliderContainer.rightTriangle.x = 144
    }

    MouseArea {
        anchors.fill: parent
    }

    GrayDescriptionText {
        id : elementName

        anchors {
            verticalCenter: buttonFirstArea.verticalCenter
            left : parent.left
            leftMargin: 50
        }

        text : "<p align=\"center\">U Shaped Pad<br/>
                Creator</p>"
        font.pixelSize: 25
    }

    Image {
        id : iconImage

        smooth : true

        fillMode: Image.PreserveAspectFit
        anchors {
            top : elementName.bottom
            topMargin: 30

            left : elementName.left
        }
        width : 100
        height : 100
    }

    Row {
        id : buttonFirstArea

        spacing : 10
        anchors {
            top : parent.top
            topMargin : 30
            right : parent.right
            rightMargin: 50
        }

        ImageBasedButton {
            id : cancelButton

            source : "qrc:/QmlResources/cancel.png"

            onCustomClicked:
            {
                SettingsPageComponentsSettings.currentDraggablePad.restoreValuesToBasicState()
                hide()
            }
        }

        ImageBasedButton {
            id : acceptButton

            source : "qrc:/QmlResources/accept.png"

            onCustomClicked: {
                SettingsPageComponentsSettings.saveValues(stiffnessSlider.value,
                                                          mmValues.text,
                                                          sliderContainer.leftTriangle.x,
                                                          sliderContainer.rightTriangle.x,
                                                          sliderContainer.depthValue)
                hide()
            }
        }
    }

    StyledButton {
        id : removePad

        height : 50
        width : 150

        titleText : "<p align=\"center\">Remove<br/>pad</p>"

        text {
            textFormat: Text.RichText
        }

        anchors {
            top : buttonFirstArea.bottom
            topMargin: 25
            right: customizePad.left
            rightMargin: 10
        }

        onCustomClicked: {
            SettingsPageComponentsSettings.currentDraggablePad.destroy()
        }
    }

    StyledButton {
        id : customizePad

        height : 50
        width : 150

        titleText : "<p align=\"center\">Customize<br/>pad shape</p>"

        text {
            textFormat: Text.RichText
        }

        anchors {
            top : buttonFirstArea.bottom
            topMargin: 25
            horizontalCenter: buttonFirstArea.horizontalCenter
        }

        onCustomClicked: {
            sigCustomizePad()
        }
    }

    Item {
        id : heightDepthComponentRow

        anchors {
            top : iconImage.bottom
            topMargin: 60
            left : parent.left
            leftMargin: 10
            right : parent.right
            rightMargin: 10
        }

        GrayDescriptionText {
            id : descriptionText

            text : "Height/Depth"
            font.pixelSize: 22

            anchors {
                verticalCenter: sliderContainer.verticalCenter
                verticalCenterOffset: -5
                left : parent.left
            }
        }

        DoublePointersSlider {
            id : sliderContainer

            anchors {
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: 30
            }

            onValueChanged: {
                mmValues.text = value
            }

            Binding {
                target: sliderContainer.leftTriangle
                property: "x"
                value: SettingsPageComponentsSettings.currentDraggablePad !== null ?
                           SettingsPageComponentsSettings.currentDraggablePad.m_HeightDepthXL :
                           sliderContainer.m_BasicLeftTriangleX
            }

            Binding {
                target: sliderContainer.rightTriangle
                property: "x"
                value: SettingsPageComponentsSettings.currentDraggablePad !== null ?
                           SettingsPageComponentsSettings.currentDraggablePad.m_HeightDepthXR :
                           sliderContainer.m_BasicRightTriangleX
            }
        }

        TextFieldWithUnit {
            id : mmValues

            height : 30
            width : 70

            horizontalAlignment : Text.AlignHCenter
            prefixMargin: 2
            readOnly: true

            anchors {
                verticalCenter: sliderContainer.verticalCenter
                right : parent.right
                rightMargin: 0
            }

            Binding {
                target: mmValues
                property: "text"
                value: SettingsPageComponentsSettings.currentDraggablePad !== null   ?
                           (SettingsPageComponentsSettings.currentDraggablePad.m_Height+
                            SettingsPageComponentsSettings.currentDraggablePad.m_Depth) : ""
            }

            Text {
                id : deltaPrefix
                text : "△"

                font.pixelSize: 15

                anchors {
                    verticalCenter: parent.verticalCenter
                    left : parent.left
                    leftMargin: 6
                }
            }
        }
    }

    GrayDescriptionText {
        id : stiffnesText

        anchors {
            top: heightDepthComponentRow.bottom
            topMargin: 50
            left : parent.left
            leftMargin: 10
        }

        text : "Stiffness"
        font.pixelSize: 22
    }

    DefaultValueIcon {
        anchors {
            verticalCenter: stiffnesText.verticalCenter
            left : stiffnesText.right
            leftMargin: 35
        }
    }

    Row {
        id : stiffnessRow

        spacing : 30

        anchors {
            top : stiffnesText.bottom
            topMargin: 30
            left : parent.left
            leftMargin: 40
            right : parent.right
            rightMargin: 40
        }

        StyledSlider {
            id : stiffnessSlider

            maximumValue: 90
            stepSize : 10

            width : parent.width-70
            slider.onValueChanged: {
                if(stiffnessSliderText != null)
                    stiffnessSliderText.text = value
            }
        }

        TextFieldWithUnit {
            id : stiffnessSliderText

            validator: IntValidator {bottom: 0; top: 90;}
            height : 30
            width : 70

            prefix: "PSI"
            slider : stiffnessSlider

            anchors
            {
                verticalCenterOffset: -10
                verticalCenter: stiffnessSlider.verticalCenter
            }

            Binding {
                target: stiffnessSliderText
                property: "text"
                value: SettingsPageComponentsSettings.currentDraggablePad !== null ?
                           SettingsPageComponentsSettings.currentDraggablePad.m_Stiffness : ""
            }
        }
    }

    Connections {
        target : SettingsPageComponentsSettings

        onResetSliderValue:
        {
            resetSlider()
        }

    }

    onHide : {
        SettingsPageComponentsSettings.currentDraggablePad =  null
    }

}
