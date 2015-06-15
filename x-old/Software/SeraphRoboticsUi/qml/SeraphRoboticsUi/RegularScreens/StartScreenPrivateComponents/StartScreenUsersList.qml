import QtQuick 2.4

import "../../Components"

// User list which is used in page 12.
GroupBoxTemplate {
    id : userList

    height : 200
    width : 500

    property alias listView : listView
    property alias currentIndex: listView.currentIndex

    Component {
        id: highlight

        Rectangle {
            width: userList.width-10; height: 30
            color: "#abadd3"; radius: 5
            y: listView.currentItem.y;

            anchors {
                horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Text {
        id : descriptionText

        anchors {
            bottom: listView.top
            bottomMargin: 2
            left : parent.left
            leftMargin: 25
        }

        font.pixelSize: 20

        color : "#999999"
        text : "Prescriber Name"
    }

    ListView {
        id : listView

        clip : true

        anchors {
            top : parent.top
            topMargin : 80
            bottom : parent.bottom
            bottomMargin: 10
        }
        width: parent.width

        model : usersListModel.list
        highlight : highlight
        highlightFollowsCurrentItem: false

        delegate: Rectangle {
            width : parent.width
            height : 30

            color : "transparent"
            Text {
                font.pixelSize: 17

                color :index == listView.currentIndex ? "#ffffff" : "#999999"
                anchors {
                    verticalCenter: parent.verticalCenter
                    left : parent.left
                    leftMargin: 20
                }

                text: model.name.title+" "+name.firstName + " " +name.lastName
            }
            MouseArea {
                anchors.fill: parent

                onClicked: {
                    listView.currentIndex = index
                }
            }
        }
    }
}
