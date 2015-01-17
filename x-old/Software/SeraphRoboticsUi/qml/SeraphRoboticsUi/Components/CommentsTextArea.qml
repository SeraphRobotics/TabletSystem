import QtQuick 2.4

Rectangle {
    id : mainWindow

    height : 250
    width : 350

    radius : 10
    border.color: "#cccccc"

    property real widthScaleValue: 350/1280
    property real heightScaleValue: 250/800
    property real leftMarginScaleValue: 150/1280

    property alias text : textArea.text

    gradient : Gradient
    {
    GradientStop { position: 0.0; color: "#ffffff" }
    GradientStop { position: 0.8; color: "#ffffff" }
    GradientStop { position: 1; color: "#ffffff" }
}

TextEdit {
    id : textArea

    font.pixelSize:   20
    color : "#666666"
    property int  lineHeight : 24

    height : parent.height-10
    width : mainWindow.width-60

    /// @note Text.RichText occured error during text resize,text not resizable correctly
    //    textFormat: Text.RichText

    wrapMode : Text.WrapAnywhere
    clip : true

    selectByMouse : true
    selectByKeyboard: true

    anchors {
        left : parent.left
        leftMargin: 30
        top : parent.top
        topMargin: 10
    }
}

Column {
    id : textLinesElement

    clip : true

    anchors {
        top : parent.top
        topMargin: 5
    }

    Repeater {
        model: (mainWindow.height-10)/textArea.lineHeight

        Rectangle {
            width: mainWindow.width-30; height: textArea.lineHeight
            color : "transparent"

            anchors {
                left : parent.left
                leftMargin: 30
            }

            Component.onCompleted: {
                if(parent != null)
                {
                    anchors.left = parent.left
                }
            }

            Rectangle {
                width : parent.width
                height : 1
                color : "#b3b3b3"
                anchors {
                    bottom: parent.bottom
                    bottomMargin: -2
                }
            }
        }
    }
}
}
