import QtQuick 2.4

import "../RegularScreens/SettingsPagePrivateComponents/" 1.0

// Item used to fill View3d items background. In this case we can unselect current pad by clicking
// outside the pad. Enabled this area only when settingsPageManager.shellModificationsState !== "padCreator"
MouseArea {
    id : currentPadUnselectArea

    anchors.fill: parent

    enabled: SettingsPageComponentsSettings.currentDraggablePad  ===
             null || (SettingsPageComponentsSettings.currentDraggablePad != null &&
                    settingsPageManager.shellModificationsState === "padCreator"
                      )  ? false : true

    onClicked: {
        if(enabled)
            SettingsPageComponentsSettings.unselectCurrentDraggablePad()
    }
}
