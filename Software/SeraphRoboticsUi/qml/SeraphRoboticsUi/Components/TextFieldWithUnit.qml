import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TextField {
    id : textField

    text : slider != null ? slider.value : 0
    height : 30
    width : 60

    property Item slider:null

    validator: IntValidator {bottom: 0; top: 20;}
    maximumLength: 2

    property string prefix : " mm"
    property int prefixMargin : 15

    Text {
        id : mmUnit
        text : prefix

        anchors {
            verticalCenter: parent.verticalCenter
            right : parent.right
            rightMargin: prefixMargin
        }
    }

    onTextChanged:  {
        if(slider != null)
            slider.value = parseInt(textField.text)
    }
}
