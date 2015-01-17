import QtQuick 2.4

Text {
    id : textElement

    textFormat: Qt.RichText

    signal linkClicked()

    wrapMode : Text.Wrap

    property alias mouseArea : mouseArea

    color : "#666666"

    font {
        pixelSize: 30
    }

    MouseArea {
        id : mouseArea

        drag.target: textElement

        anchors.fill : parent

        onClicked: {
            linkClicked()
        }
    }

}
