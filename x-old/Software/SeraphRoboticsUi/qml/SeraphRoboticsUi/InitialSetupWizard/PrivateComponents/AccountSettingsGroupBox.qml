import QtQuick 2.4

import "../../Components"
import ".."

GroupBoxTemplate {
    id : accountSettings

    height : 220
    width : 510

    title: "Account Settings"

    property alias pinRequireValue : pinRequire.checked
    property alias loginByPrescriberValue : loginByPrescriber.checked
    property alias autoAssignPatientsValue : autoAssignPatients.checked

    Column {
        anchors {
            verticalCenter: parent.verticalCenter
            verticalCenterOffset: 15
            horizontalCenter: parent.horizontalCenter
        }

        spacing : 10

        ToggleButton {
            id : pinRequire

            title: "require pin to login"
            checked : true
        }
        ToggleButton{
            id : loginByPrescriber

            title: "login by prescriber"
        }
        ToggleButton {
            id : autoAssignPatients

            title: "auto-assign patients"
            checked: true
        }
    }
}

