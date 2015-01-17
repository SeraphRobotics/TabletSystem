import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../Components"
import ".."

Rectangle {   
    property bool checked: radioButt.checked
    property int checkInd: 0

    width: parent.width
    height: 50
    color : "transparent"

    signal showFileDialog()

    StyledRadioButton {
        id: radioButt

        group: editAccountSetupPageGroup
        anchors.verticalCenter: parent.verticalCenter

        checked: (checkInd == index)

    }

    StyledTextInput {
        id : prescriberName

        readOnly: true

        backgroundNormalBorderColor : "lightgray"

        text : name.title+" "+name.firstName + " " +name.lastName

        anchors {
            right : pinNumber.left
            rightMargin: 15
            left : parent.left
            leftMargin: 30
            verticalCenter:  parent.verticalCenter
        }
    }

    StyledTextInput {
        id : pinNumber

        readOnly: true

        width : 140
        backgroundNormalBorderColor : "lightgray"

        text : password

        anchors {
            right : photoIcon.left
            rightMargin: 15
            verticalCenter:  parent.verticalCenter
        }
    }


    Rectangle {
        id : photoIcon

        anchors {
            right : parent.right
            rightMargin: 30
            verticalCenter:  parent.verticalCenter
        }
        height: parent.height * 0.95
        width: height
        radius : 10

        color : "transparent"

        border {
            width : 1
            color : "#b3b3b3"
        }

        Image {
            anchors.fill: parent
            source: iconPath
            fillMode: Image.Stretch
        }
    }
}

