import QtQuick 2.4

import "../../../../Components"
import "../DraggableElementComponents" 1.0
import "../../"

Image {

    width: draggablePad.handleSize
    height: draggablePad.handleSize
    visible : draggablePad.frameElementsVisible

    property Item draggablePad : null

    source : "qrc:/exampleImages/tri-square.svg"

    property real lastX
    property real lastY

    property alias mouseArea : mouseArea

    MouseArea {
        id : mouseArea


        onPressed:
        {
            var obj = mapToItem(draggablePad.parent, mouse.x, mouse.y)
            lastX = obj.x
            lastY = obj.y

            ////@note catch starting position to move pad history array.
            if(settingsPageManager.shellModificationsState === "startMovePad"
                    && SettingsPageComponentsSettings.movePadHistory.movePadHistoryCount === -1)
            {
                SettingsPageComponentsSettings.movePadHistory.pushIntoSizeHistory(draggablePad.
                                                                                  basicWidth,
                                                                                  draggablePad
                                                                                  .basicHeight,
                                                                                  draggablePad.basicX,
                                                                                  draggablePad.basicY,
                                                                                  draggablePad.exampleImage.x,
                                                                                  draggablePad.exampleImage.y,
                                                                                  draggablePad.width,
                                                                                  draggablePad.height,
                                                                                  draggablePad.exampleImage.rotation)
            }
        }

        onReleased:
        {
            if(settingsPageManager.shellModificationsState === "startMovePad")
            {
                SettingsPageComponentsSettings.movePadHistory.pushIntoSizeHistory(draggablePad
                                                                                  .basicWidth,
                                                                                  draggablePad
                                                                                  .basicHeight,
                                                                                  draggablePad.basicX,
                                                                                  draggablePad.basicY,
                                                                                  draggablePad.exampleImage.x,
                                                                                  draggablePad.exampleImage.y,
                                                                                  draggablePad.width,
                                                                                  draggablePad.height,
                                                                                  draggablePad.exampleImage.rotation)
            }
        }


        anchors.fill: parent
    }
}

