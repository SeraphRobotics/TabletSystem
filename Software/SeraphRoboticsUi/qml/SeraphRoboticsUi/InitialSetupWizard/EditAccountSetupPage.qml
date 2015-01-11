import QtQuick 2.4
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.0
import QtQuick.Dialogs 1.1

import ".."
import "../Components"
import "./PrivateComponents/"

PageTemplate {

    active: false

    FileDialog {
        id: fileDialog

        property string caller: ""

        title: "Please choose a file"
        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            if (caller == "Add") {
                addAccountWindow.imageSource = fileDialog.fileUrl
            } else {
                editAccountWindow.imageSource = fileDialog.fileUrl
            }
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    NavigationButton {
        id : leftNavigationButton

        buttonText : "back & undo changes"

        onButtonClicked:  {
            stateManager.setState(stateManager.previousState)
            usersListModel.resetList()
        }

        Component.onCompleted: {
            leftOrRightButton("left")
        }
    }

    NavigationButton {
        id : rightNavigationButton

        buttonText : "save & log on"
        onButtonClicked:  {
            stateManager.setState(stateManager.previousState)
            usersListModel.saveUserList()

        }
        Component.onCompleted: {
            leftOrRightButton("right")
        }
    }

    GrayDescriptionText {
        id : headerText

        anchors {
            top : leftNavigationButton.bottom
            horizontalCenter: parent.horizontalCenter
        }
        text : "Account Setup"
    }

    GroupBoxTemplate {
        id : listContainer

        opacity: 1


        height : 370
        width : 870

        headerTextFontPixelSize : 22
        headerHeight : 50

        anchors {
            centerIn: parent
        }

        title:   "Prescriber Accounts"

        Rectangle {
            id : headerTextElements

            color : "transparent"

            anchors {
                top : listContainer.header.bottom
                topMargin: 15
                left : parent.left
                leftMargin: 15
                right : parent.right
                rightMargin: 15
                bottom : accountList.top
                bottomMargin: 5
            }

            Text {
                id : presciberText

                color : "#999999"

                font {
                    pixelSize: 19
                    bold : true
                }

                anchors {
                    left : parent.left
                    leftMargin: 220
                    verticalCenter:  parent.verticalCenter
                }
                text : "Prescriber Name"

            }
            Text {
                id : pinText

                font {
                    pixelSize: 19
                    bold : true
                }

                color : "#999999"

                anchors {
                    left : presciberText.right
                    leftMargin: 230
                    verticalCenter:  parent.verticalCenter
                }

                text : "Pin Num"
            }
            Text {
                id : photoThumbanil

                font {
                    pixelSize: 19
                    bold : true
                }

                color : "#999999"

                anchors {
                    left : pinText.right
                    leftMargin: 40
                    verticalCenter:  parent.verticalCenter
                }
                text : "Photo"
            }
        }

        ExclusiveGroup {
            id: editAccountSetupPageGroup
        }

        ListView {
            id : accountList

            property int checkInd: 0

            model : usersListModel.list

            highlightMoveVelocity : 0

            anchors {
                top : listContainer.header.bottom
                topMargin: 50
                bottom : buttonArea.top
                bottomMargin: 15
                left : parent.left
                leftMargin: 15
                right : parent.right
                rightMargin: 15
            }

            clip : true

            delegate: EditAccountListDelegate {
                id: editDelegate1

                onShowFileDialog: {
                    fileDialog.visible = true
                }
                onCheckedChanged: {
                    if (checked) {
                        accountList.checkInd = index
                        accountList.currentIndex = index
                    }
                }

            }
        }
        AccountSetupSelectButtons {
            id : buttonArea

            onDeleteClicked: {
                listContainer.opacity = 0
                confiramationDialog.opacity = 1
            }

            onAddClicked: {
                listContainer.opacity = 0
                addAccountWindow.opacity = 1

                addAccountWindow.resetInputs()
            }

            onEditClicked: {
                listContainer.opacity = 0
                editAccountWindow.opacity = 1
            }

            anchors {
                right : parent.right
                rightMargin: 20
                bottom : parent.bottom
                bottomMargin: 25
            }
        }

        Behavior on opacity {
            PropertyAnimation { }
        }
    }

    GroupBoxTemplate {
        opacity: 0
        visible: opacity

        id: confiramationDialog

        anchors.centerIn: listContainer
        width: listContainer.width / 3 * 2
        height: listContainer.height / 1.5

        title: "Confirm Deletion"

        StyledButton {
            id: styledCancel

            titleText: "Cancel"
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.bottomMargin: parent.height / 10
            anchors.rightMargin: parent.width / 10
            width: parent.width / 3
            height: parent.height / 4

            onCustomClicked: {
                listContainer.opacity = 1
                confiramationDialog.opacity = 0
            }
        }

        StyledButton {
            titleText: "Delete"
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.bottomMargin: parent.height / 10
            anchors.leftMargin: parent.width / 10
            width: parent.width / 3
            height: parent.height / 4

            onCustomClicked: {
                listContainer.opacity = 1
                confiramationDialog.opacity = 0
                usersListModel.removeAt(accountList.checkInd)
            }
        }

        Text {
            id: confirmText

            color : "black"

            text: "Upon deleting this account, all associated patient files\nwill be transferred to"

            font {
                pixelSize: 18
                bold : true
            }

            anchors {
                top: parent.header.bottom
                topMargin: 10
                right: parent.right
                left: parent.left

            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignHCenter


        }

        UsersTitleComboBox {
            id: confirmComboBox
            objectName: "confirmComboBox"

            anchors {
                top: confirmText.bottom
                horizontalCenter: parent.horizontalCenter
            }
        }

        Behavior on opacity {
            NumberAnimation { }
        }
    }


    AddAccountWindow {
        id: addAccountWindow
        opacity: 0
    }

    EditAccountWindow {
        id: editAccountWindow
        opacity: 0
    }


}

