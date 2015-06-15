import QtQuick 2.2

Image {
    id : blueComponent
    source : "qrc:/QmlResources/blue_arrow.png"
    opacity : 0

    function startBlinking()
    {
        blinkingAnimation.running = true
    }

    SequentialAnimation {
        id : blinkingAnimation

        running: false
        loops:  Animation.Infinite

        NumberAnimation { target: blueComponent; property: "opacity"; to: 0; duration: 2000 }
        NumberAnimation { target: blueComponent; property: "opacity"; to: 1; duration: 2000 }
    }

}
