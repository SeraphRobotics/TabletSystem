import QtQuick 2.4
import ".."
import "../../Components"

GroupBoxTemplate {
    title: "Patients"

    property real widthScaleValue: 500/1280

    width : widthScaleValue*parent.width

    anchors {
        top : parent.top
        topMargin: 85
        left : parent.left
        leftMargin: 30
        bottom : parent.bottom
        bottomMargin : 100
    }

    function searchPatient()
    {
        listView.currentIndex = -1
        patientsListModel.setSearchListViaSpecificKey(textInput.text)
        patientsListModel.currentIndex = findIndexFromOrginaList()
    }

    function setIndex(listViewElement, m_index)
    {
        listViewElement.currentIndex = m_index
        patientsListModel.currentIndex = m_index
    }

    function findIndexFromOrginaList()
    {
        if(searchinglistView.currentIndex === -1)
            return -1

        for(var i=0; i <patientsListModel.size(); i++)
        {
            if(patientsListModel.getSpecificItem(i).id ===
                    patientsListModel.searchPatientsList[searchinglistView.currentIndex].id)
                return i
        }
    }

    Rectangle {
        id : container

        color : "transparent"

        width : parent.width
        height : textInput.height

        anchors {
            top : patientsList.header.bottom
            topMargin: 15
            horizontalCenter:  parent.horizontalCenter
        }

        StyledTextInput {
            id : textInput

            horizontalAlignment : TextInput.AlignLeft

            inputHeight : textInput.height
            height : 40

            focus : true

            placeholderText: " write patient name.."

            anchors {
                left : parent.left
                leftMargin: 10
                right : searchButton.left
                rightMargin: -10
                top : searchButton.top
            }

            onTextChanged: {
                searchPatient()
            }


            Keys.onReturnPressed:  {
                searchPatient()
            }
        }
        Image {
            id : searchButton

            source : "qrc:/QmlResources/search-button.png"
            smooth : true

            height : textInput.height

            anchors {
                right : parent.right
                rightMargin: 10
            }

            Text {
                text : "Search"
                anchors.centerIn: parent
                color : "white"
                font.pixelSize: 17
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    searchPatient()
                }
            }
        }
    }

    ListView {
        id : searchinglistView

        model : patientsListModel.searchPatientsList

        clip : true

        anchors {
            top : container.bottom
            topMargin : 5
            bottom : middleBorder.top

            bottomMargin: 5
        }

        width: parent.width

        highlightMoveDuration : 0

        delegate: Rectangle {
            width : parent.width-20
            height : textValues.contentHeight+10

            radius: 5

            anchors {
                left : parent.left
                leftMargin: 10
            }
            color :  index == searchinglistView.currentIndex ? "#abadd3" : "transparent"
            GrayDescriptionText {
                id : textValues

                font.pixelSize: 17

                height : parent.height-5

                anchors {
                    verticalCenter: parent.verticalCenter
                    left : parent.left
                    leftMargin: 20
                    right : parent.right
                    rightMargin: 20
                }

                text: model.name.title+" "+name.firstName + "," +name.lastName
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    searchinglistView.currentIndex = index
                    patientsListModel.currentIndex = findIndexFromOrginaList()
                    listView.currentIndex = -1
                }
            }
        }
    }

    Rectangle {
        id : middleBorder

        color : "#b3b3b3"

        opacity : 0.8

        height : 1

        anchors {
            top : parent.top
            topMargin: parent.width*3.5/7
            left : parent.left
            leftMargin: 25
            right : parent.right
            rightMargin: 25
        }
    }

    Item {
        id : sortButtonsRow

        anchors {
            top : middleBorder.bottom
            topMargin: 15

            left : middleBorder.left
            leftMargin: 20
            right : middleBorder.right

        }

        StyledComboBox {
            id : alphabeticSort

            fontPixelSize : 15
            textLeftMargin : 25
            model: [ "A-Z", "Z-A" ]

            onActivated: {
                patientsListModel.sortWithSpecificOrder(textAt(index))
            }

            width : 100
        }

        StyledComboBox {
            id : dateSort

            width : 180

            visible: true

            fontPixelSize : 15
            textLeftMargin : 10
            model: [ "newest-oldest", "oldest-newest" ]

            onActivated: {
                patientsListModel.sortWithSpecificOrder(textAt(index))
            }

            anchors {
                right : parent.right
            }
        }
    }

    ListView {
        id : listView

        model : patientsListModel.list

        clip : true

        anchors {
            top : sortButtonsRow.bottom
            topMargin : 50
            bottom : parent.bottom

            bottomMargin: 30
        }

        width: parent.width

        currentIndex: patientsListModel.currentIndex

        highlightMoveDuration : 0

        delegate: Rectangle {

            width : parent.width-20
            height : userNameText.contentHeight+10

            radius: 5

            anchors {
                left : parent.left
                leftMargin: 10
            }

            color :  index == listView.currentIndex ? "#abadd3" : "transparent"

            GrayDescriptionText {
                id : userNameText

                font.pixelSize: 17

                height : parent.height-5

                anchors {
                    verticalCenter: parent.verticalCenter
                    left : parent.left
                    leftMargin: 20
                    right : dateTextElement.left
                    rightMargin: 50
                }

                text: model.name.title+" "+name.firstName + "," +name.lastName
            }

            GrayDescriptionText {
                id : dateTextElement

                font.pixelSize: 17

                width : 100
                height : parent.height

                anchors {
                    verticalCenter: parent.verticalCenter
                    right : parent.right
                    rightMargin: 20
                }
                text:  Qt.formatDateTime(model.dateTime, "d/M/yyyy")
            }

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    setIndex(listView, index)
                    searchinglistView.currentIndex = -1
                }
            }
        }
    }

    Component.onDestruction: {
        if(patientsListModel != null)
            patientsListModel.restartListModel()
    }

}
