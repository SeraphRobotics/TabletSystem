import QtQuick 2.4

import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
Row {

    spacing : 30
    property alias title: toggleButtonTitle.text
    property alias checked: toogleButton.checked

    Text {
        id : toggleButtonTitle

        font.pixelSize: 20

        width : 180 /// set here  harcdoded value to reach all switch elements in the same line
        /// not matter how text is long or short , we need to know that text not have more than 150 px size width

        color : "#666666"

        anchors {
            verticalCenter: toogleButton.verticalCenter
        }
        text : ""
    }

    Switch{
        id : toogleButton

        height : 30
        width : 150
        style :
            SwitchStyle {
            groove: Rectangle {
                implicitWidth: 100
                implicitHeight: 30
                radius: 12
                smooth: true

                border.color: "lightgray"
                border.width: 1

                gradient : toogleButton.checked == true ? onStateGradient : offStateGradient


                Gradient {
                    id : offStateGradient

                    GradientStop { position: 0.0; color: "#8a8a8a" }
                    GradientStop { position: 0.5; color: "#c8c8c8" }
                    GradientStop { position: 1.0; color: "#c8c8c8" }
                }

                Gradient {
                    id : onStateGradient

                    GradientStop { position: 0.0; color: "#dd3a35" }
                    GradientStop { position: 0.5; color: "#ffaf5b" }
                    GradientStop { position: 1.0; color: "#ffb05c" }
                }


                Text {
                    text : "on"

                    opacity: (toogleButton.checked) ? 1 : 0

                    color : "#ffffff"

                    font.pixelSize: 19

                    anchors {
                        left : parent.left
                        leftMargin : 10
                        verticalCenter: parent.verticalCenter
                    }

                    Behavior on opacity {
                        NumberAnimation { }
                    }
                }

                Text {
                    text : "off"

                    opacity: (toogleButton.checked) ? 0 : 1

                    color : "#ffffff"

                    font.pixelSize: 19

                    anchors {
                        right : parent.right
                        rightMargin : 10
                        verticalCenter: parent.verticalCenter
                    }

                    Behavior on opacity {
                        NumberAnimation { }
                    }
                }
            }
            handle : Rectangle{
                implicitWidth: Math.round((parent.parent.width - padding.left - padding.right)/2)
                implicitHeight: control.height - padding.top - padding.bottom
                radius: 12
                smooth: true


                border.color: "lightgray"
                border.width: 1
                gradient : Gradient {

                    GradientStop { position: 0.5; color:  "#ffffff" }
                    GradientStop { position: 0.51; color:  "#e7e8e9" }
                    GradientStop { position: 0.75; color:  "#f2f3f3" }
                    GradientStop { position: 1.0; color:  "#f0f0f1"}
                }
            }
        }
    }
}
