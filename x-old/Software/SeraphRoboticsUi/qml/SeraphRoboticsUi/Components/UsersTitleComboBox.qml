import QtQuick 2.4

StyledComboBox {
    property string currentUser : objectName ===  "confirmComboBox" ?
                                      usersListModel.getSpecificItem(accountList.currentIndex).name.title+" "+
                                      usersListModel.getSpecificItem(accountList.currentIndex).name.firstName+" "+
                                      usersListModel.getSpecificItem(accountList.currentIndex).name.lastName : ""

    currentIndex: 0

    width: parent.width / 2

    function recreateModel()
    {
        comboBoxusersListModel.clear()
        for(var i = 0; i<usersListModel.size(); i++)
        {
            var userName = usersListModel.getSpecificItem(i).name.title+" "+
                    usersListModel.getSpecificItem(i).name.firstName+" "+
                    usersListModel.getSpecificItem(i).name.lastName

            if( (userName.toString() !== currentUser && objectName === "confirmComboBox" )
                    || objectName ===  "allUsersList")
                comboBoxusersListModel.append({ "text": userName })
        }

        if(objectName ===  "confirmComboBox")
        {
            comboBoxusersListModel.append({ "text":  "unasigned"})
            comboBoxusersListModel.append({ "text":  "deleted permanently"})
        }

        if(objectName ===  "allUsersList")
        {
            console.log("Searching for "+currentUser+".")
            selectSpecificItemViaContent(currentUser)
        }
    }

    ListModel
    {
        id : comboBoxusersListModel
    }

    model : comboBoxusersListModel

    onVisibleChanged:
    {
        recreateModel()
    }

    Component.onCompleted: {
        recreateModel()
    }
}
