import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id : main

    color : "transparent"

    property alias maximumValue : slider.maximumValue
    property alias minimumValue : slider.minimumValue
    property alias value : slider.value
    property alias stepSize : slider.stepSize

    property alias slider: slider
    property alias minusButton : minusButton
    property alias plusButton: plusButton

    property int customDefaultValue: 6

    property int firstTickMarksY : 0

    property string tickmarkColor :  "#666666"

    property string leftText : "pillow"
    property string middleText : "cork"
    property string rightText : "brick"

    height : slider.height+20

    property int  upTextValue : 7
    property int  bottomTextValue : 15

    property bool selectFirstElement : false

    ImageBasedButton {
        id : minusButton

        anchors {
            left : parent.left
            verticalCenter: slider.verticalCenter
        }
        source : "qrc:/QmlResources/btn-minus.png"

        onCustomClicked: {
            if( slider.value == minimumValue)
                return
            slider.value -= slider.stepSize
        }
    }

    ImageBasedButton {
        id : plusButton

        anchors {
            right : parent.right
            verticalCenter: slider.verticalCenter
        }
        source : "qrc:/QmlResources/btn-plus.png"

        onCustomClicked: {
            if( slider.value == maximumValue)
                return
            slider.value += slider.stepSize
        }
    }

    Slider {
        id : slider

        width : parent.width-60

        anchors {
            left : minusButton.right
            leftMargin: 10
        }

        tickmarksEnabled : true
        maximumValue: 10
        minimumValue: 0
        value: 7
        stepSize: 1

        onValueChanged: print(value)

        style: SliderStyle {
            groove:

                Rectangle {
                implicitWidth: 200
                implicitHeight: 80

                color : "transparent"

                Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: 2
                    color: "#666666"
                    radius: 8

                    anchors {
                        bottom : parent.bottom
                        bottomMargin: 35
                    }
                }
            }

            handle: Image {
                anchors {
                    bottom : parent.top
                    bottomMargin: -8
                    horizontalCenter: parent.horizontalCenter
                }

                source : "qrc:/QmlResources/pointer.png"

                onWidthChanged: {
                    firstTickMarksY = width/2
                }

            }

            tickmarks: Repeater {
                id: repeater

                model: control.stepSize > 0 ? 1
                                              + (control.maximumValue - control.minimumValue) /
                                              control.stepSize : 0

                Rectangle {
                    id : delegate

                    color: (selectFirstElement === true &&
                            tickmarkColor === "transparent" &&
                            index == 1) ? "#666666" : tickmarkColor


                    width: 1 ; height:(index-(Math.abs(minimumValue)/stepSize))*stepSize
                                      === value  ? 27 : 19
                    anchors {
                        bottom : parent.bottom
                        bottomMargin: (index-(Math.abs(minimumValue)/stepSize))*stepSize
                                      === value ? 8 : 16
                    }
                    y: repeater.height
                    x: styleData.handleWidth / 2 +
                       index * ((repeater.width - styleData.handleWidth) / (repeater.count-1))

                    function checkIfShouldShowTextDescription()
                    {
                        if( index === 0
                                || index === repeater.count-1
                                || index === parseInt(repeater.count/2))
                            return true;
                        else
                            return false;
                    }

                    function setText()
                    {
                        if(index === 0)
                            return leftText
                        else if(index === parseInt(repeater.count/2) )
                            return middleText
                        else if(index === repeater.count-1)
                            return rightText

                        return ""
                    }

                    Image {
                        id : defaultValue

                        visible : index == customDefaultValue ? true : false
                        anchors {
                            topMargin: -10
                            top: parent.top
                            horizontalCenter: parent.horizontalCenter
                        }
                        source : "qrc:/QmlResources/pointer-default.png"
                    }

                    GrayDescriptionText {

                        font.pixelSize : 11

                        anchors {
                            top : parent.bottom
                            topMargin: (index-(Math.abs(minimumValue)/stepSize))*stepSize
                                       === value ?  upTextValue : bottomTextValue
                            horizontalCenter: parent.horizontalCenter
                        }

                        visible :checkIfShouldShowTextDescription()
                        text : setText()
                    }
                }
            }
        }
    }
}
