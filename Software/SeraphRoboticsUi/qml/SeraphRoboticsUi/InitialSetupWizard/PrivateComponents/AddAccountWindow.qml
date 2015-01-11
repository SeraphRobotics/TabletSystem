import QtQuick 2.4

import "../../Components"

GroupBoxTemplate {
    id: addAccountWindow

    property alias imageSource: addAccountWindowImage.source

    opacity: 0
    visible: opacity

    anchors.centerIn: listContainer
    width: listContainer.width / 2
    height: listContainer.height * 1.5

    title: "Add Account"

    function resetInputs() {
        title.curIndx = 0
        fName.textFiled.text = ""
        lName.textFiled.text = ""
        pas.textFiled.text = ""
        addAccountWindowImage.source = ""

        title.focus = true
    }

    StyledButton {
        id: addAccountWindowCancelButton

        titleText: "Cancel"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.height / 10
        anchors.rightMargin: parent.width / 10
        width: parent.width / 3
        height: parent.height / 7

        onCustomClicked: {
            listContainer.opacity = 1
            addAccountWindow.opacity = 0
        }
    }

    StyledButton {
        titleText: "Add"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: parent.height / 10
        anchors.leftMargin: parent.width / 10
        width: parent.width / 3
        height: parent.height / 7

        onCustomClicked: {
            usersListModel.appendNewUser
                    (fName.inputText,
                     lName.inputText,
                     title.inputText,
                     "",
                     pas.inputText,
                     addAccountWindowImage.source,
                     cert.inputText)

            listContainer.opacity = 1
            addAccountWindow.opacity = 0

        }
    }

    Column {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.header.bottom
            bottom: addAccountWindowCancelButton.top
        }
        spacing: 2
        Rectangle {
            id: title

            property alias curIndx: textInp.currentIndex
            property string inputText: textInp.currentText

            color: "transparent"

            width: parent.width; height: parent.height / 7;
            anchors.horizontalCenter: parent.horizontalCenter

            Text {
                id: titleStyledText

                anchors.left: parent.left
                anchors.right: textInp.left
                anchors.margins: parent.width / 20

                height: parent.height

                text: "Title"
                color : "#666666"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter

                font {
                    pixelSize: 16
                    bold : true
                }
            }

            StyledComboBox {
                id: textInp

                currentIndex: 0

                anchors.right: parent.right
                anchors.rightMargin: parent.width / 3 * 1.55
                anchors.verticalCenter: parent.verticalCenter
                width: parent.width / 5; height: parent.height / 1.5

                model: [ "Dr.", "Mr.", "Mrs.","Miss.","Ms." ]

            }

        }

        StyledTextInputWText {
            id: fName

            width: parent.width; height: parent.height / 7;
            text: "First Name"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        StyledTextInputWText {
            id: lName

            width: parent.width; height: parent.height / 7;
            text: "Last Name"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        StyledTextInputWText {
            id: pas

            width: parent.width; height: parent.height / 7;
            text: "PIN"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        StyledTextInputWText {
            id: cert

            width: parent.width; height: parent.height / 7;
            text: "Degree/\nCertification"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        StyledTextInputWText {
            id: photo

            width: parent.width; height: parent.height / 7;
            text: "Photo"
            anchors.horizontalCenter: parent.horizontalCenter
            textFiled.visible: false

            Rectangle {
                id : photoIcon

                anchors {
                    left : parent.textFiled.left
                    verticalCenter:  parent.verticalCenter
                }

                height: parent.height
                width: height
                radius : 10

                color : "transparent"

                border {
                    width : 1
                    color : "#b3b3b3"
                }

                Image {
                    id: addAccountWindowImage
                    fillMode: Image.Stretch

                    anchors.centerIn: parent
                    anchors.fill: parent
                }
            }

            StyledButton {
                id : uploadPhotoButton

                width : 150
                height : 40
                titleText : "upload photo"

                anchors {
                    right : parent.right
                    rightMargin: 15
                    verticalCenter:  parent.verticalCenter
                }

                onCustomClicked: {
                    fileDialog.caller = "Add"
                    fileDialog.visible = true
                }
            }
        }

    }

    Behavior on opacity {
        NumberAnimation { }
    }
}
