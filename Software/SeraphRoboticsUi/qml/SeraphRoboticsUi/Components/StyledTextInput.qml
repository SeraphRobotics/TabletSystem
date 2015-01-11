import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TextField {

    style : styleNormalInput

    horizontalAlignment : TextInput.AlignHCenter
    verticalAlignment: TextInput.AlignVCenter

    property int deafultFontSize : 14
    property int inputHeight: 35

    property string backgroundColor : "white"
    property string backgroundNormalBorderColor : "#cccccc"

    property string welcomeTextColor : "#e6e6e6"

    signal validate()

    property alias styleNormalInput : styleNormalInput
    property alias styleErrorInput: styleErrorInput


    Gradient {
        id :backgroundGradient
        GradientStop { position: 0.6; color: "#ffffff" }
        GradientStop { position: 1.0; color: "#e9e9ea" }
    }

    Component {
        id :  styleNormalInput

        TextFieldStyle {
            placeholderTextColor :welcomeTextColor
            font.pixelSize : deafultFontSize

            background: Rectangle {
                id : m_background

                gradient : backgroundGradient
                radius: 10
                implicitWidth: 100
                implicitHeight: inputHeight
                border {
                    color: backgroundNormalBorderColor
                    width: 1
                }
            }
        }
    }
    Component {
        id :  styleErrorInput

        TextFieldStyle {
            placeholderTextColor :welcomeTextColor
            font.pixelSize : deafultFontSize
            background: Rectangle {
                id : m_background

                gradient : backgroundGradient

                radius: 10
                implicitWidth: 100
                implicitHeight: inputHeight

                border {
                    color: "#f15a24"
                    width: 2
                }
            }
        }
    }

    Keys.onReturnPressed: {
        validate()
    }

    onVisibleChanged: {
        if(visible)
            style = styleNormalInput
    }
}
