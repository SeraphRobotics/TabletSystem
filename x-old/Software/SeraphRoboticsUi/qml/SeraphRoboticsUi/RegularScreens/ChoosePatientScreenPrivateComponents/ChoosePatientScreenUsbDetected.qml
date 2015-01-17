import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import ".."
import "../../Components"

Rectangle {

    visible: false
    anchors.fill: parent
    color : "transparent"

    anchors {
        top : parent.header.bottom
        topMargin: 35
    }

    GrayDescriptionText {
        id : pageTitle

        anchors {
            left : parent.left
            leftMargin: 20
            top : parent.top
            topMargin: 30
        }
        font.pixelSize: 20

        text : "Files on USB Drive"
    }

    Rectangle {
        id : headerContainer

        color : "transparent"

        anchors {
            top : pageTitle.bottom
            topMargin: 30
            left : usbItemsList.left
            leftMargin: 0
        }

        width : usbItemsList.width-160
        height : textTimestamp.height+10


        GrayDescriptionText {
            id : textTimestamp

            font {
                pixelSize: 16
                bold: true
            }
            anchors {
                top : parent.top
                left : parent.left
                leftMargin: 60/560*parent.width
            }
            text : "Timestamp"
        }
        GrayDescriptionText {
            font {
                pixelSize: 16
                bold: true
            }
            anchors {
                left : parent.left
                leftMargin: 265/560*parent.width
            }
            text : "Type"
        }
        GrayDescriptionText {
            font {
                pixelSize: 16
                bold: true
            }
            text : "Status"

            anchors {
                left : parent.left
                leftMargin: 410/560*parent.width
            }
        }
    }

    ListView {
        id : usbItemsList

        model : UsbFakeModel {
        }
        highlightMoveVelocity : 0
        anchors {
            top : headerContainer.bottom
            topMargin: 2
            bottom : border.top
            bottomMargin: 100/700*parent.height
            horizontalCenter: parent.horizontalCenter
        }
        width : parent.width-40
        clip : true
        delegate: ChoosePatientScreenDelegate {
            listView : usbItemsList
        }
    }
    Row {
        id : buttonsRow

        spacing : 150
        anchors {
            top : usbItemsList.bottom
            topMargin: 15
            horizontalCenter: parent.horizontalCenter
        }
        StyledButton {
            id : deleteButton

            anchors.verticalCenter: parent.verticalCenter
            width : 120
            titleText : "delete"

        }
        StyledButton {
            id : addScan

            width : 160
            height : 60
            titleText : "add scan to\nexisting patient"
        }
    }

    Rectangle {
        id : border

        height : 2
        anchors {
            left : parent.left
            leftMargin: 25
            right : parent.right
            rightMargin: 25
            verticalCenter: parent.verticalCenter
        }
        color : "lightgray"
    }

    StyledButton {
        id : loadScan

        anchors {
            top : border.bottom
            topMargin: 15
            left : buttonsRow.left
        }
        width : 160
        height : 60
        titleText : "load scan as \nnew patient"
    }

    GrayDescriptionText {
        id : patientNameInfo

        text : "patient name"

        font.pixelSize: 20

        anchors {
            top : border.bottom
            topMargin: 5
            left : loadScan.right
            leftMargin: 70
        }
    }

    StyledTextInput {
        id : patientNameInput

        textColor: "#666666"
        welcomeTextColor : "#666666"

        anchors {
            top : patientNameInfo.bottom
            topMargin: 10
            left : patientNameInfo.left
            right : parent.right
            rightMargin: 100
        }
    }

    GrayDescriptionText {
        id : commentsTextInfo

        text : "comments"

        font.pixelSize: 20

        anchors {
            top : patientNameInput.bottom
            topMargin: 5
            left : loadScan.right
            leftMargin: 70
        }
    }

    CommentsTextArea {
        anchors {
            top : commentsTextInfo.bottom
            topMargin: 10
            left : patientNameInput.left
            right : patientNameInput.right
            bottom : parent.bottom
            bottomMargin: 20
        }
        text : usbItemsList.model.get(usbItemsList.currentIndex).comment
    }
}
