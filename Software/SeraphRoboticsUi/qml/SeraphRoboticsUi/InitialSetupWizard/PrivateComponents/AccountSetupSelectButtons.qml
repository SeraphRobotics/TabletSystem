import QtQuick 2.4
import ".."
import "../../Components"

Row {
    signal deleteClicked
    signal addClicked
    signal editClicked

    property alias editAccountButton : editAccount

    spacing : 80

    StyledButton {
        id : deleteAccount

        width : 180
        titleText : "Delete Account"

        onCustomClicked: {
            deleteClicked()
        }
    }

    StyledButton {
        id : editAccount

        width : 210
        titleText : "Edit Account"

        onCustomClicked: {
            editClicked()
        }
    }

    StyledButton {
        id : addAccount

        width : 210
        titleText : "Add New Account"

        onCustomClicked: {
            addClicked()
        }
    }
}

