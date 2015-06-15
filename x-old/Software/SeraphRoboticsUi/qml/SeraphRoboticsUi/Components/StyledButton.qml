import QtQuick 2.4

Rectangle {
    id : button

    width: 200
    height: 50
    smooth : true
    radius : 15

    property alias titleText: title.text
    property alias text : title

    property string buttonType: "normal"
    property string buttonText: "";
    property bool mouseclick: false

    signal customClicked()
    signal release()
    signal customPressed()

    gradient: normalGradient
    border {
        color : "#666666"
        width : 1
    }

    Gradient {
        id : normalGradient

        GradientStop { position: 0.5; color: buttonType == "startButton" ? "#546ef6" : "#ffffff" }
        GradientStop { position: 0.51; color: buttonType == "startButton" ? "#3345a8" : "#e7e8e9" }
        GradientStop { position: 0.75; color: buttonType == "startButton" ? "#374bae" : "#f2f3f3" }
        GradientStop { position: 1.0; color: buttonType == "startButton" ?"#607dda" : "#f0f0f1"}
    }

    Gradient {
        id : pressedGradient

        GradientStop { position: 0.0; color: buttonType == "startButton" ?"#2e3192" : "#f0f0f1"}
        GradientStop { position: 1.0; color: buttonType == "startButton" ?"#546ef6" : "#ffffff"}
    }

    Text {
        id : title

        color : "#666666"
        font.pixelSize: 19
        text : "START"
        renderType: Text.NativeRendering

        anchors {
            verticalCenter: parent.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }
    }

    MouseArea {
        anchors.fill: parent

        cursorShape : Qt.PointingHandCursor

        onClicked:{
            if(button.opacity != 1)
                return
            console.log("Styled button clicked.")
            customClicked()
        }
        onPressed: {
            if(button.opacity != 1)
                return
            gradient = pressedGradient
            customPressed()
        }
        onReleased: {
            if(button.opacity != 1)
                return
            gradient = normalGradient
            release()
        }
    }
}
