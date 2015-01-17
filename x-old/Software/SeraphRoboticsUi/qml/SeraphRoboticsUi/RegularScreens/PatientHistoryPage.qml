import QtQuick 2.4

import "../Components" 1.0
import "PatientHistoryScreenPrivateComponents"

import ".."

/// Page 18 from pdf.
PageTemplate {
    id : mainContainer

    NavigationBarWithButtons {
        id : topNavigationPanel

        z : mainContainer.z+1

        anchors
        {
            left : parent.left
            leftMargin: 50

            right : parent.right
            rightMargin : 25

            top : parent.top
            topMargin: 10
        }

        leftButton.buttonText: "1:choose patient"
        middleButton.buttonText: "step 2: patient history"
        rightButton.buttonText: "3: settings"


        onLeftButtonClicked:
        {
            stateManager.setState("choosePatientState")
        }
        onRightButtonClicked:
        {
            stateManager.setState("settingsPageScreen")
        }
        onMiddleButtonClicked: {
            TutorialWindowPopup.showPopup()
        }
    }


    GrayDescriptionText {
        id : commentsInfo

        font.pixelSize: 20
        anchors {
            bottom: firstCommentsScreen.top
            bottomMargin: 5
            horizontalCenter: firstCommentsScreen.horizontalCenter
        }

        text : "comments"
    }

    CommentsTextArea {
        id : firstCommentsScreen

        height : heightScaleValue*parent.height
        width : widthScaleValue*parent.width

        anchors {
            left : patientNameInput.right
            leftMargin: leftMarginScaleValue*parent.width
            top : availableScans.top
            topMargin: -10
        }

        text : patientsListModel.
        getSpecificItem(patientsListModel.currentIndex).
        scansList[scansListView.currentIndex].comment
    }

        GrayDescriptionText {
            id : commentsInfoForSecondArea

            font.pixelSize: 20

            anchors {
                bottom: secondCommentsScreen.top
                bottomMargin: 5
                horizontalCenter: secondCommentsScreen.horizontalCenter
            }

            text : "comments"
        }

        CommentsTextArea {
            id : secondCommentsScreen

            height : heightScaleValue*parent.height
            width : widthScaleValue*parent.width

            anchors {
                top : firstCommentsScreen.bottom
                topMargin: 50

                left : patientNameInput.right
                leftMargin: leftMarginScaleValue*parent.width
            }

            text : patientsListModel.
            getSpecificItem(patientsListModel.currentIndex).
            rxList[rxListView.currentIndex].comment
        }

            Text {
                id : textInputDescription

                color: "#666666"

                font {
                    pixelSize: 20
                }

                anchors {
                    left : parent.left
                    leftMargin: 70
                    bottom : prescriberText.top
                    bottomMargin: 20
                }

                text : "patient name"
            }


            StyledTextInput {
                id : patientNameInput

                property real widthScaleValue: 350/1280
                width : widthScaleValue*parent.width
                horizontalAlignment : Text.AlignLeft

                deafultFontSize: 14
                textColor: "#666666"
                welcomeTextColor : "#666666"

                readOnly: true

                text :  patientsListModel.getSpecificItem(patientsListModel.currentIndex).
                name.firstName +
                " "+patientsListModel.getSpecificItem(patientsListModel.currentIndex)
                .name.lastName

                anchors
                {
                    verticalCenter: textInputDescription.verticalCenter
                    left: textInputDescription.right
                    leftMargin : 100
                }
            }

            Text  {
                id : prescriberText

                color: "#666666"

                font {
                    pixelSize: 20
                }

                anchors {
                    left : parent.left
                    leftMargin: 70
                    bottom : availableScans.top
                    bottomMargin: 30
                }

                text : "assigned prescriber"
            }

            UsersTitleComboBox {
                id : allUsersList
                objectName: "allUsersList"

                currentUser: usersListModel.getNameById(patientsListModel.
                                                        getSpecificItem
                                                        (patientsListModel.currentIndex).doctorId)

                property real widthScaleValue: 350/1280
                width : widthScaleValue*parent.width

                anchors {
                    verticalCenter: prescriberText.verticalCenter
                    right: patientNameInput.right
                }
            }

            GroupBoxTemplate {
                id : availableScans

                property real heightScaleValue: 170/800
                height : heightScaleValue*parent.height

                title : "Scans Available"

                anchors {
                    left : prescriberText.left
                    right : patientNameInput.right
                    bottom : buttonArea.top
                    bottomMargin: 30
                }

                CustomListView {
                    id : scansListView

                    objectName: "scansListView"
                    model : patientsListModel.getSpecificItem(patientsListModel.currentIndex).scansList

                    clip : true

                    anchors {
                        top : availableScans.top
                        topMargin : 50
                        bottom : parent.bottom

                        bottomMargin: 30
                    }
                }
            }

            Row {
                id : buttonArea

                spacing : 50
                anchors {

                    horizontalCenter: availableScans.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                StyledButton {
                    id : deleteButton

                    width : 150
                    titleText : "delete"

                }
                StyledButton {
                    id : logout

                    anchors {
                        verticalCenter: deleteButton.verticalCenter
                    }

                    width : 200
                    height : 70
                    titleText : "create new Rx\nfrom selected scan"
                }
            }

            GroupBoxTemplate {
                id : rxAvailable

                property real heightScaleValue: 170/800
                height : heightScaleValue*parent.height

                title : "Rx Available"

                anchors {
                    left : prescriberText.left
                    right : patientNameInput.right
                    top : buttonArea.bottom
                    topMargin: 20
                }

                CustomListView {
                    id : rxListView

                    objectName: "rxListView"
                    model : patientsListModel.getSpecificItem(patientsListModel.currentIndex).rxList

                    clip : true

                    anchors {
                        top : rxAvailable.top
                        topMargin : 50
                        bottom : parent.bottom

                        bottomMargin: 30
                    }
                }
            }

            Row {
                spacing : 50
                anchors {

                    horizontalCenter: rxAvailable.horizontalCenter
                    top : rxAvailable.bottom
                    topMargin: 15
                }

                Column {
                    spacing : 15

                    StyledButton {
                        id : deleteRx

                        width : 150
                        titleText : "delete"
                    }
                    StyledButton {
                        id : editButton

                        width : 150
                        titleText : "edit"
                    }
                }

                StyledButton {
                    id : transferRx

                    anchors {
                        verticalCenter: parent.verticalCenter
                    }

                    width : 200
                    height : 70
                    titleText : "transfer Rx to USB\ndrive for fabrication"
                }
            }
        }

