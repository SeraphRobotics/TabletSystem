import QtQuick 2.4

import "../../Components"

GroupBoxTemplate {

    property real widthScaleValue: 550/1280
    property real heightScaleValue: 200/800

    height : heightScaleValue*parent.height
    width : widthScaleValue*parent.width

    title:   "Currently logged in as"

    Image {
        id: iconImage

        fillMode : Image.PreserveAspectFit

        anchors {

            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 5
            left : parent.left
            leftMargin: 40
        }

        source : usersListModel.currentIndex == -1 ? "" :
                                                usersListModel.getSpecificItem(usersListModel.currentIndex).iconPath

        Component.onCompleted: {
            if(iconImage.sourceSize.height > 200)
                iconImage.height = 130
        }
    }

    GrayDescriptionText {
        id : accountName

        anchors {
            right : parent.right
            rightMargin: 60
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 5
        }
        color : "#999999"
        font {
            bold : true
            pixelSize: 22
        }

        text :usersListModel.currentIndex == -1 ? "" :
                                             "<p align=\"right\">"+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.title+"<br/>"+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.firstName+" "+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.lastName+"</p>"
    }
}
