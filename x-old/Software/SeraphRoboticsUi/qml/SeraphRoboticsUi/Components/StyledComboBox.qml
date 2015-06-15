import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

ComboBox {

    height : 35

    property int fontPixelSize : 13
    property int textLeftMargin: 4

    function selectSpecificItemViaContent(searchingValue)
    {
        console.log("Searching for: "+searchingValue)
        for(var i = 0; i<model.count; i++)
        {
            if(model.get(i).text === searchingValue)
            {
                currentIndex = i
                return
            }
        }
    }

    style : ComboBoxStyle {
        background: Rectangle {
            smooth : true
            radius : 15
            border {
                color: "#cccccc"
                width: 1
            }
            gradient : Gradient {
                id :backgroundGradient
                GradientStop { position: 0.6; color: "#ffffff" }
                GradientStop { position: 1.0; color: "#e9e9ea" }
            }
        }
        label: Item {
            implicitWidth: textitem.implicitWidth + 20
            baselineOffset: textitem.y + textitem.baselineOffset
            Text {
                id: textitem
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: textLeftMargin
                anchors.rightMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: control.currentText
                color: "#999999"
                elide: Text.ElideRight
                font.pixelSize: fontPixelSize
            }
        }
    }

    Image {
        smooth : true
        source : "qrc:/QmlResources/arrow_down.png"

        anchors {
            right : parent.right
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }
    }
}
