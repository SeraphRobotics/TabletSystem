import QtQuick 2.4
import "../Components"
import ".."

PageTemplate {

    active: false

    NavigationButton {
        id : rightButton

        opacity : 0

        onButtonClicked: {
            if(fadeIn.running == false)
                stateManager.setState("customerNumberPageState")
        }
        Component.onCompleted: {
            leftOrRightButton("right")
        }
    }

    BlueArrowComponent {
        id : blueArrow

        anchors {
            top : parent.top
            topMargin: 128
            right : parent.right
            rightMargin: 214
        }
    }

    BlackDescriptionText {
        id : topText

        opacity : 0

        anchors {
            bottom : wifiIcon.top
            bottomMargin : 0
            horizontalCenter: parent.horizontalCenter
        }

        text :
            "<p align=\"center\">Please conect to the internet by clicking<br/>
                     the wireless icon<br/>
                     and logging into your network.<br/></p>"
    }

    Image {
        id : wifiIcon

        opacity : 0

        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 50
        }

        source : "qrc:/QmlResources/wifi.png"
    }

    BlackDescriptionText {
        id : bottomText

        opacity : 0

        anchors {
            top : wifiIcon.bottom
            topMargin : 15
            horizontalCenter: parent.horizontalCenter
        }

        text :
            "<p align=\"center\">You must be logged in to the internet to use<br/>
        the software at all times.</p>"
    }

    SequentialAnimation {
        id : fadeIn

        running: true

        ParallelAnimation {
            NumberAnimation { target: topText; property: "opacity"; to: 1; duration: 3000 }
        }

        ParallelAnimation {
            NumberAnimation { target: wifiIcon; property: "opacity"; to: 1; duration: 1000 }
            NumberAnimation { target: bottomText; property: "opacity"; to: 0.9; duration: 2000 }
        }

        ParallelAnimation {
            NumberAnimation { target: bottomText; property: "opacity"; to: 1; duration: 1000 }
            NumberAnimation { target: blueArrow; property: "opacity"; to: 1; duration: 1000 }
            NumberAnimation { target: rightButton; property: "opacity"; to: 1; duration: 1000 }
        }

        onRunningChanged: {
            if(running === false)
            {
                blueArrow.startBlinking()
            }
        }
    }

}
