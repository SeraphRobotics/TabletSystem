import QtQuick 2.4
import QtGraphicalEffects 1.0

import "../" 1.0

Item {
    id : canvasContainer

    width : parent.width
    height : parent.height

    property alias canvasPaintArea : canvasPaintArea
    property alias scale: scale

    property real centerX : (width/2)
    property real centerY : (height/2)

    property variant basicValues : QtObject
    {
    property int basicAngle
    property int basicX
    property int basicY
}

clip : true

// Function used to catch pad from drawing area.
function prepareScaledImage()
{

    var rootObject = canvasPaintArea

    var newWidth = rootObject.width
    var newHeight = rootObject.height

    var image = rootObject.context.getImageData(rootObject.canvasWindow.x,
                                                rootObject.canvasWindow.y,
                                                rootObject.canvasWindow.width,
                                                rootObject.canvasWindow.height)

    rootObject.context.clearRect(0, 0,
                                 rootObject.canvasWidth, rootObject.canvasHeight)
    rootObject.context.save();

    //Remove scaling
    rootObject.context.scale(1/scale.xScale, 1/scale.yScale)

    rootObject.context.drawImage(image, 0, 0)
    rootObject.context.restore();
    rootObject.requestPaint()
    rootObject.width = rootObject.canvasWidth
    rootObject.height = rootObject.canvasHeight
    rootObject.canvasWindow = Qt.rect(0,
                                      0,
                                      newWidth/scale.xScale,
                                      newHeight/scale.yScale)
}


// This timer is using for duplicating canvas cleanup action.
// We need this workaround because of the bug in Qt Canvas.
// The clearRect() function doesn't remove some painted areas sometimes, so
// we need to trigger timer after clearRect() to make sure it fixes the problem.
Timer {
    id : clearCanvas

    interval: 50

    onTriggered:
    {
        console.log("Clear Canvas again.")
        canvasPaintArea.context.clearRect(0, 0,
                                          canvasPaintArea.width, canvasPaintArea.height);
    }
}

Scale {
    id: scale

    yScale: SettingsPageComponentsSettings.m_CurrentSelectedArea.view3d
    .height/SettingsPageComponentsSettings.startingSideElementHeight

    xScale: SettingsPageComponentsSettings.m_CurrentSelectedArea.view3d
    .width/SettingsPageComponentsSettings.startingSideElementWidth
}

Canvas {
    id: canvasPaintArea

    visible : typeof (SettingsPageComponentsSettings.m_ShellModificationsComponentPtr) !== "undefined" ?
                  (((settingsPageManager.shellModificationsState == "padCreator" ||
                     settingsPageManager.shellModificationsState == "startMovePad") &&
                    mainWindow.state === "select") ? true : false) : false

    property real lastX
    property real lastY
    property color color: "#474754"
    property alias objectRotation : objectRotation

    property real canvasWidth: 1600
    property real canvasHeight: 1600

    width : canvasWidth
    height : canvasHeight

    enabled: visible

    anchors
    {
        centerIn: parent
    }

    onVisibleChanged:
    {
        /// Clear canvas when leaving padCreatorState.
        if(visible === false)
        {
            clearDrawArea()
            // Leave "padCreator" state when canvas changes visibility to false
            if(settingsPageManager.shellModificationsState != "up")
                settingsPageManager.shellModificationsState = "down"
        }
    }

    transform: [
        Rotation {
            id: objectRotation

            function restoreBasicValues()
            {
                angle = basicValues.basicAngle
                origin.x = basicValues.basicX
                origin.y = basicValues.basicY
            }

            Component.onCompleted: {
                basicValues.basicAngle = angle
                basicValues.basicX = origin.x
                basicValues.basicY = origin.y
            }
        }
    ]

    function _clearDrawArea(type)
    {
        if(context !== null)
        {
            canvasPaintArea.context.clearRect(0, 0,
                                              canvasPaintArea.width, canvasPaintArea.height);
            if(type !== "notCleanHistory")
                clearCanvas.running = true
        }
    }

    onPaint: {
        if( typeof (SettingsPageComponentsSettings.m_ShellModificationsComponentPtr) === "undefined")
        return

        //Undo/redo catch action.
        if(lastX === drawMouseArea.mouseX &&
           lastY === drawMouseArea.mouseY)
        return

        // Paint event.
        if(SettingsPageComponentsSettings.m_ShellModificationsComponentPtr
           .padCreator.drawButton.state === "selected")
        {
            var ctx = getContext('2d')
            ctx.globalCompositeOperation = 'multiply';
            ctx.fillStyle = 'rgb(255,0,255)';
            ctx.lineWidth = SettingsPageComponentsSettings.canvasPaintPenWidth*scale.xScale
            ctx.strokeStyle = canvasPaintArea.color
            ctx.beginPath()
            ctx.moveTo(lastX, lastY)
            lastX = drawMouseArea.mouseX
            lastY = drawMouseArea.mouseY

            ctx.fill()

            SettingsPageComponentsSettings.drawingAreaDetection.setValue(lastX,
                                                                         lastY,
                                                                         canvasContainer)

            ctx.lineTo(lastX, lastY)
            ctx.stroke()
        }

        // Erase event.
        else if (SettingsPageComponentsSettings.m_ShellModificationsComponentPtr
                 .padCreator.eraseButton.state === "selected")
        {
            lastX = drawMouseArea.mouseX
            lastY = drawMouseArea.mouseY
            canvasPaintArea.context.clearRect(lastX, lastY,
                                              15, 15)
            canvasPaintArea.requestPaint()
        }
    }

    MouseArea {
        id: drawMouseArea

        anchors.fill: parent
        enabled: canvasPaintArea.visible

        // Value used to mark that paint event has occurred.
        property bool paintedSomething: false

        onPressed:
        {
            canvasPaintArea.lastX = mouseX
            canvasPaintArea.lastY = mouseY
        }
        onClicked:
        {
            if(settingsPageManager.shellModificationsState === "padCreator")
            return
        }
        onPositionChanged:
        {
            paintedSomething = true
            canvasPaintArea.requestPaint()
        }
        onReleased:
        {
            var eraseOrDraw =
            SettingsPageComponentsSettings.m_ShellModificationsComponentPtr
            .padCreator.drawButton.state === "selected"
            ||
            SettingsPageComponentsSettings.m_ShellModificationsComponentPtr
            .padCreator.eraseButton.state === "selected"

            if(paintedSomething === true && eraseOrDraw === true)
            {
                cPush()
                paintedSomething = false
            }
        }
    }

    Component.onCompleted:
    {
        // Prepare canvas context on application start.
        context.getContext('2d')
    }
}

// Color effect for pen.
ColorOverlay {
    id : drawingLineColorEffectElement

    width : canvasPaintArea.width
    height: canvasPaintArea.height
    source: canvasPaintArea

    rotation: canvasPaintArea.rotation

    transform: [
        Rotation {
            id: drawingObjectRotation

            angle: canvasPaintArea.objectRotation.angle
            axis: canvasPaintArea.objectRotation.axis
            origin.x: canvasPaintArea.objectRotation.origin.x
            origin.y: canvasPaintArea.objectRotation.origin.y
        }
    ]

    anchors
    {
        left : canvasPaintArea.left
        top  : canvasPaintArea.top
    }

    color:  "#4359ce"
    clip : true
}

}
