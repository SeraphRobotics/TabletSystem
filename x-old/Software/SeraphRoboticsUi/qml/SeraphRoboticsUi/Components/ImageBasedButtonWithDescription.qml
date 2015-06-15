import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle {

    property alias buttonElement : buttonElement
    property alias textElement : textElement
    property alias buttonOpacity : buttonElement.opacity

    property alias selectEffect : buttonElement.selectEffect

    color : "transparent"

    width : buttonElement.width+30
    height : buttonElement.height+30

    ImageBasedButton {
        id : buttonElement
    }

    GrayDescriptionText {
        id : textElement

        font.pixelSize: 18
        text : "draw"

        anchors {
            bottom : parent.bottom
            horizontalCenter: buttonElement.horizontalCenter
        }
    }

    onVisibleChanged: {
        if(visible === false)
            state = ""
    }

    states : [
        State {
            name : "selected"
            PropertyChanges { target: buttonElement; state: "selected" }
        }
    ]

}
