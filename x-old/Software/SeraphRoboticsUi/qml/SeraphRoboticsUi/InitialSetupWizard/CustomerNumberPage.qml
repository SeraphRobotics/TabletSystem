import QtQuick 2.4

import "../Components"
import ".."

PageTemplate {
    id : customerScreenPage

    active: false

    function validateInput()
    {
        /// @todo change to regex
        var pinText = deleteWrongChars()
        if(usersListModel.checkIfValueExist(pinText.toString(), "id") === true
                && pinText.length !== 0)
            stateManager.setState("accountSetupState")
    }

    function deleteWrongChars()
    {
        return customNumberInput.text.toString().replace("{","").replace("}","");
    }

    function validateText()
    {
        return usersListModel.checkIfValueExist(deleteWrongChars(), "id")
    }

    function validateLength()
    {
        return deleteWrongChars().length >= 36
    }

    NavigationButton {

        onButtonClicked:  {
            if(validateLength())
                validateInput()
            else blink.restart()
        }

        Component.onCompleted: {
            leftOrRightButton("right")
        }
    }

    BlackDescriptionText {
        id: topInfoText

        anchors {
            bottom: customNumberInput.top
            bottomMargin : 30
            horizontalCenter: customNumberInput.horizontalCenter
        }

        text: "You must now input your customer number for verification:"
    }

    StyledTextInput {
        id : customNumberInput

        anchors.centerIn: parent
        height : 35
        width : 300

        style : ( validateLength() && !(validateText()) ) ?  customNumberInput.styleErrorInput
                                                          :  customNumberInput.styleNormalInput
        onValidate: {
            if(validateLength())
                validateInput()
            else blink.restart()
        }
    }

    Text {
        id : wrongTextInput

        anchors {
            horizontalCenter: customNumberInput.horizontalCenter
            top : customNumberInput.bottom
            topMargin: 5
        }

        color : "#f15a24"
        font.pixelSize: 15

        text : "incorrect customer number"

        visible: validateLength() && !(validateText())
    }

    Text {
        id : wrongLengthInput

        anchors {
            horizontalCenter: customNumberInput.horizontalCenter
            top : customNumberInput.bottom
            topMargin: 5
        }

        color : "#f15a24"
        font.pixelSize: 15

        text : "incoret length"

        opacity: 0

        SequentialAnimation {
            id: blink

            NumberAnimation { target: wrongLengthInput; properties: "opacity"; to: 1; duration: 1250}
            NumberAnimation { target: wrongLengthInput; properties: "opacity"; to: 0; duration: 1250}
        }
    }

    ImageBasedButton {
        id : inputPassIcon

        visible: validateLength()

        width : sourceSize.width
        height : sourceSize.height

        source : validateText() === true ?
                     "qrc:/QmlResources/blue-checkbox.png" :  "qrc:/QmlResources/exclamation-mark.png"

        anchors {
            left : customNumberInput.right
            leftMargin : 20
            verticalCenter: customNumberInput.verticalCenter
        }

        onCustomClicked: {
            validateInput()
        }
    }

    BlackDescriptionText{
        id: bottomInfoText

        anchors {
            top: customNumberInput.bottom
            topMargin : 30
            horizontalCenter: customNumberInput.horizontalCenter
        }
        text: "Customer Number (from confirmation email)."
    }

}
