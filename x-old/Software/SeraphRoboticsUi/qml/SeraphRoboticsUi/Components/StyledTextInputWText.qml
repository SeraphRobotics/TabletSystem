import QtQuick 2.4

Rectangle {
    property alias text: styledText.text

    property string inputText: textInp.text

    property alias textFiled: textInp

    color: "transparent";
    width: parent.width; height: parent.height / 5;
    anchors.horizontalCenter: parent.horizontalCenter

    Text {
        id: styledText

        anchors.left: parent.left
        anchors.right: textInp.left
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

    StyledTextInput {
        id: textInp

        anchors.right: parent.right
        anchors.rightMargin: parent.width / 20
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width / 3 * 2; height: parent.height / 1.5
    }
}
