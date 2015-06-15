import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtGraphicalEffects 1.0

Rectangle {
    width: 200
    height: 25

    color : "transparent"

    property alias descriptionText: radioButton.text
    property alias checked : radioButton.checked
    property alias group : radioButton.exclusiveGroup

    signal areaClicked()

    RadioButton {
        id : radioButton

        anchors.verticalCenter: parent.verticalCenter

        onClicked: {
            areaClicked()
        }

        style: RadioButtonStyle {
            indicator: Rectangle {
                implicitWidth: 16
                implicitHeight: 16
                radius: 9

                border.color: control.activeFocus ? "transparent" : "grey"
                border.width: 1

                Rectangle {
                    anchors.fill: parent

                    color: "transparent"
                    radius: parent.radius

                    opacity: (control.checked) ? 1 : 0
                    visible: control.checked

                    RadialGradient {
                        anchors.fill: parent
                        cached: true
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#FDA759"}
                            GradientStop { position: 0.42; color: "#E34E3C"}
                            GradientStop { position: 0.46; color: "#C2282E"}
                            GradientStop { position: 0.47; color: "transparent" }
                        }
                    }
                    Behavior on opacity {
                        NumberAnimation { duration: 750 }
                    }
                }
            }

            label:
                Item {
                implicitWidth: text.implicitWidth + 2
                implicitHeight: text.implicitHeight
                baselineOffset: text.y + text.baselineOffset

                anchors {
                    left : parent.left
                    leftMargin: 10
                }

                Rectangle {
                    anchors.fill: text
                    anchors.margins: -10
                    anchors.leftMargin: -8
                    anchors.rightMargin: -8
                    visible: control.activeFocus
                    height: 6
                    radius: 3
                    color: "#224f9fef"
                    border.color: "#47b"
                    opacity: 0.6
                }
                GrayDescriptionText {
                    id : text

                    text: control.text
                    font.pixelSize : 20
                }
            }
        }
    }
}
