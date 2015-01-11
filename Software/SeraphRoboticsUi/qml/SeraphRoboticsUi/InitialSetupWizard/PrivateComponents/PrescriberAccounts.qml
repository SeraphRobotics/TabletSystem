import QtQuick 2.4

import "../../Components"

GroupBoxTemplate {
    id : prescriberAccounts

    property real widthScaleValue: 550/1280
    property real heightScaleValue: 150/800

    height: heightScaleValue*parent.height
    width: widthScaleValue*parent.width

    title: "Prescriber Accounts"


    GrayDescriptionText {
        id : presciberAccount

        anchors {
            left : parent.left
            leftMargin: 20
            top : prescriberAccounts.header.bottom
            topMargin: 20
        }

        text :usersListModel.currentIndex == -1 ? "" : "<b>Prescriber name</b><br/>"+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.title+" "+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.firstName+" "+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).name.lastName
        color : "#999999"
        font {
            pixelSize: 22
        }
    }
    GrayDescriptionText {
        id : passwordText

        anchors {
            right : parent.right
            rightMargin: 20
            top : prescriberAccounts.header.bottom
            topMargin: 20
        }

        text :usersListModel.currentIndex == -1 ? "" : "<b>Pin Num</b><br/>"+
                                             usersListModel.getSpecificItem(usersListModel.currentIndex).password
        color : "#999999"
        font {
            pixelSize: 22
        }
    }
}
