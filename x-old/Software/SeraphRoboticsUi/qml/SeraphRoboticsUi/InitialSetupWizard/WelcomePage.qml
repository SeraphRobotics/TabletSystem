import QtQuick 2.4

import "../Components"
import ".."

PageTemplate {
    id : welcomeScreenPage

    //@note: to make SR logo not clickable
    active: false

    NavigationButton {
        id : rightButton

        opacity : 0

        onButtonClicked: {
            if(fadeIn.running == false)
                stateManager.setState("noInternetConnection")
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
        id : welcomeText

        opacity : 0
        anchors.centerIn: parent

        text :
            "Welcome to Seraph Robotics’ Orthotics Software!"

    }
    BlackDescriptionText {
        id : detailedText

        opacity : 0
        text : "<p align=\"center\">The next few screens will allow us to tailor the<br/>
                     software to your needs and show you how easy<br/>
                    it is to create the world’s most advanced<br/>
                     robotic orthotics for your patients.</p>";

        anchors {
            top : welcomeText.bottom
            horizontalCenter:  welcomeText.horizontalCenter
        }
    }

    SequentialAnimation {
        id : fadeIn

        running: true

        ParallelAnimation {
            NumberAnimation { target: welcomeText; property: "opacity"; to: 1; duration: 3000 }
        }
        NumberAnimation { target: detailedText; property: "opacity"; to: 0.9; duration: 3000 }
        ParallelAnimation {
            NumberAnimation { target: detailedText; property: "opacity"; to: 1; duration: 1000 }
            NumberAnimation { target: rightButton; property: "opacity"; to: 1; duration: 1000 }
            NumberAnimation {  target: blueArrow; property: "opacity"; to: 1; duration: 1000 }
        }

        onRunningChanged: {
            if(running === false)
            {
                blueArrow.startBlinking()
            }
        }
    }

}
