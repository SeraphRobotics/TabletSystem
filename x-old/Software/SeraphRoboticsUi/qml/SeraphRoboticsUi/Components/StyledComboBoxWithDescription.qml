import QtQuick 2.4

Rectangle {
    property alias text: styledText.text
    property alias comboBox: comboBox

    color: "transparent";
    width: parent.width; height: parent.height / 5;
    anchors.horizontalCenter: parent.horizontalCenter

    function createModel(modelArray)
    {
        for(var i=0; i <modelArray.length; i++)
        {
            comboBoxusersListModel.append( { "text" : modelArray[i] })
        }
    }

    Text {
        id: styledText

        anchors.left: parent.left
        anchors.right: comboBox.left
        anchors.margins: parent.width / 20

        height: parent.height

        text: "Text"
        color : "#666666"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        font {
            pixelSize: 16
            bold : true
        }
    }

    StyledComboBox {
        id: comboBox

        anchors.right: parent.right
        anchors.rightMargin: parent.width / 20
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 3 * 2; height: parent.height / 1.5

        model : comboBoxusersListModel

        ListModel
        {
            id : comboBoxusersListModel
        }
    }
}
