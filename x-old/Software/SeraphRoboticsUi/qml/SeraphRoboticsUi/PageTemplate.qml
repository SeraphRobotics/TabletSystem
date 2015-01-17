import QtQuick 2.4

import "./RegularScreens/SettingsPagePrivateComponents/" 1.0

// Item used to set background for all Ui screens.
Rectangle {
    id : templateWindow

    property alias logo : logo
    property bool active: true

    signal logoButtonClicked()
    
    anchors.fill: parent

    gradient: Gradient
    {
        GradientStop { position: 0.3; color: "#ffffff" }
        GradientStop { position: 1.0; color: "#999999" }
    }

    Image {
        id : logo
        
        z : 99

        anchors {
            bottom : parent.bottom
            bottomMargin : 10
            right : parent.right
            rightMargin : 10
        }
        
        smooth : true
        height : SettingsPageComponentsSettings.m_MainRootObject != null ?
                 SettingsPageComponentsSettings.m_MainRootObject.logoHeight : 70
        width : 60

        source : "qrc:/QmlResources/Sr_logo.png"

        MouseArea {
            anchors.fill: parent

            visible: templateWindow.active === true ? true : false

            cursorShape: Qt.PointingHandCursor

            onClicked:
            {
                logoButtonClicked()
                stateManager.setState("summaryPage")
            }
        }
    }
}
