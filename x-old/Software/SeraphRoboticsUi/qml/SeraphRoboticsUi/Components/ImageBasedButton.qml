import QtQuick 2.4
import QtGraphicalEffects 1.0

Image {
    id : imageElement

    source : "qrc:/QmlResources/close.png"

    property alias selectEffect : selectEffect
    signal customClicked()
    signal customReleased()
    signal customPressed()

    width : sourceSize.width
    height : sourceSize.height

    MouseArea {
        anchors.fill: parent

        cursorShape : Qt.PointingHandCursor

        onClicked:{
            console.log("Styled button clicked.")
            customClicked()
        }
        onPressed: {
            customPressed()
        }
        onReleased: {
            customReleased()
        }
    }

    ColorOverlay {
        id : selectEffect

        visible : false
        anchors.fill: imageElement
        source: imageElement
        color: "#f15a24"
    }

    states : [
        State {
            name : "selected"
            PropertyChanges { target: selectEffect; visible: true }
        }
    ]

    onVisibleChanged: {
        if(visible === false)
            state = ""
    }


}
