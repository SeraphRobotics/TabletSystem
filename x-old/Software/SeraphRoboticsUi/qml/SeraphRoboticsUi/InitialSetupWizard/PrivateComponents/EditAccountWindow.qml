import QtQuick 2.4

import "../../Components"

GroupBoxTemplate {
    id: editAccountWindow

    property alias imageSource: editDialogImage.source

    opacity: 0
    visible: opacity

    anchors.centerIn: listContainer
    width: listContainer.width / 2
    height: listContainer.height * 1.5

    title: "Edit Account"

    function resetInputs() {
        title.curIndx = 0
        fName.textFiled.text = ""
        lName.textFiled.text = ""
        pas.textFiled.text = ""
        addDialogImage.source = ""

        title.focus = true
    }

    StyledButton {
        id: cancelButton

        titleText: "Cancel"
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: parent.height / 10
        anchors.rightMargin: parent.width / 10
        width: parent.width / 3
        height: parent.height / 10

        onCustomClicked: {
            listContainer.opacity = 1
            editAccountWindow.opacity = 0

        }
    }

    StyledButton {
        titleText: "Accept"
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.bottomMargin: parent.height / 10
        anchors.leftMargin: parent.width / 10
        width: parent.width / 3
        height: parent.height / 10

        onCustomClicked: {
            listContainer.opacity = 1
            editAccountWindow.opacity = 0

            usersListModel.updateUser(accountList.checkInd,fName.inputText,lName.inputText,
                                      title.inputText,"",pas.inputText,editDialogImage.source,cert.inputText)
        }
    }

    Column {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.header.bottom
            bottom: cancelButton.top
        }
        spacing: 2

        StyledComboBoxWithDescription {
            id: title

            width: fName.width; height: parent.height / 7;
            text: "Title"
            anchors.horizontalCenter: parent.horizontalCenter

            onVisibleChanged: {
                if(visible)
                    comboBox.selectSpecificItemViaContent(usersListModel.list[accountList.checkInd].name.title)
            }

            Component.onCompleted: {
                createModel(["Dr.", "Mr.", "Mrs.", "Miss.", "Ms."])
                comboBox.selectSpecificItemViaContent(usersListModel.list[accountList.checkInd].name.title)
            }

        }

        StyledTextInputWText {
            id: fName

            width: parent.width; height: parent.height / 7;
            text: "First Name"
            anchors.horizontalCenter: parent.horizontalCenter

            textFiled.text: usersListModel.list[accountList.checkInd].name.firstName
        }

        StyledTextInputWText {
            id: lName

            width: parent.width; height: parent.height / 7;
            text: "Last Name"
            anchors.horizontalCenter: parent.horizontalCenter

            textFiled.text: usersListModel.list[accountList.checkInd].name.lastName

        }

        StyledTextInputWText {
            id: pas

            width: parent.width; height: parent.height / 7;
            text: "PIN"
            anchors.horizontalCenter: parent.horizontalCenter

            textFiled.text: usersListModel.list[accountList.checkInd].password
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
                    id: editDialogImage
                    fillMode: Image.Stretch

                    source: usersListModel.list[accountList.checkInd].iconPath

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
                    fileDialog.caller == "Edit"
                    fileDialog.visible = true
                }
            }
        }

    }

    Behavior on opacity {
        NumberAnimation { }
    }


}
