import QtQuick 2.4

import "../../../Components"
import "../../../Components/View3d"
import "../" 1.0

Rectangle {
    id : mainWindow

    property alias position : textContainer.textElement
    property alias view3dSource : view3d.meshSource
    property alias imageBorderElement: imageBorderElement
    property alias textContainer: textContainer
    property alias view3d: view3d
    property alias rawImageSource: rawImage.source
    property alias rawImageVisible: rawImage.visible
    property alias canvasContainer : canvasContainer
    property string customizePadName: ""

    property var undoRedoHistory: []
    property var positionDrawHistory : []

    property int  cStep: -1

    property int view3dX:0
    property int view3dY:0

    signal elementClicked()

    color : "transparent"
    state : "unselect"
    z: 99

    // Function used to clear canvas area after painting was finished.
    function clearDrawArea(type)
    {
        console.log("Clear draw area")
        var rootObject = canvasContainer.canvasPaintArea

        // Restore basic settings
        rootObject.rotation = 0
        rootObject.objectRotation.restoreBasicValues()
        rootObject.width = rootObject.canvasWidth
        rootObject.height = rootObject.canvasHeight
        rootObject.canvasWindow = Qt.rect(0,
                                          0,
                                          rootObject.width,
                                          rootObject.height)


        if(type !== "notCleanHistory")
        {
            undoRedoHistory.splice(0,undoRedoHistory.length)
            positionDrawHistory.splice(0, positionDrawHistory.length)
            cStep = -1
        }

        SettingsPageComponentsSettings.drawingAreaDetection._reset()

        if(SettingsPageComponentsSettings.currentDraggablePad !== null)
            SettingsPageComponentsSettings.currentDraggablePad.opacity = 1

        rootObject._clearDrawArea("notCleanHistory")
        rootObject.requestPaint()
        if(type === "clearAndClose")
        {
            if(SettingsPageComponentsSettings.m_ShellModificationsComponentPtr !== null)
                SettingsPageComponentsSettings.m_ShellModificationsComponentPtr.hidePadCreator()
        }
    }

    function movePadStart()
    {
        useOnlyForThisPad("startMovePad")
    }

    function _saveNameAndClearArea(saveType)
    {
        if(SettingsPageComponentsSettings.currentDraggablePad != null)
            SettingsPageComponentsSettings.currentDraggablePad.m_Name = customizePadName.toString()

        clearDrawArea()
        SettingsPageComponentsSettings.m_ShellModificationsComponentPtr.hidePadCreator(saveType)
    }

    // Function used to store the current state of canvas.
    function cPush()
    {
        var rootObject = canvasContainer.canvasPaintArea

        cStep++;

        if (cStep < undoRedoHistory.length)
        {
            undoRedoHistory.length = cStep;
            positionDrawHistory.length = cStep
        }

        if(rootObject.context === null)
            rootObject.getContext('2d')

        // Catch current pen position.
        var positions = {
            minX: SettingsPageComponentsSettings.drawingAreaDetection.minX,
            minY: SettingsPageComponentsSettings.drawingAreaDetection.minY,
            maxX: SettingsPageComponentsSettings.drawingAreaDetection.maxX,
            maxY: SettingsPageComponentsSettings.drawingAreaDetection.maxY
        };

        positionDrawHistory.push(positions)
        undoRedoHistory.push(rootObject.context.getImageData(0,0,rootObject.canvasWidth,
                                                             rootObject.canvasHeight))
    }

    // Function used to revert canvas latest canvas changes.
    function cUndo()
    {
        var rootObject = canvasContainer.canvasPaintArea
        if (cStep > 0)
        {
            cStep--

            rootObject.context.clearRect(0, 0,
                                         rootObject.width, rootObject.height)
            rootObject.context.drawImage(undoRedoHistory[cStep],
                                         0,
                                         0
                                         )

            // Revert pen coordinates.
            SettingsPageComponentsSettings.drawingAreaDetection.
            resetValues(positionDrawHistory[cStep].maxX,
                        positionDrawHistory[cStep].maxY,
                        positionDrawHistory[cStep].minX,
                        positionDrawHistory[cStep].minY)

            rootObject.requestPaint()
        }
        SettingsPageComponentsSettings.movePadHistory.undoSizeHistory()
    }

    // Function used to redo the canvach changes.
    function cRedo()
    {
        var rootObject = canvasContainer.canvasPaintArea

        if (cStep < undoRedoHistory.length-1)
        {
            cStep++

            rootObject.context.clearRect(0, 0,
                                         rootObject.width, rootObject.height)
            rootObject.context.drawImage(undoRedoHistory[cStep],
                                         0,
                                         0
                                         )

            SettingsPageComponentsSettings.drawingAreaDetection.
            resetValues(positionDrawHistory[cStep].maxX,
                        positionDrawHistory[cStep].maxY,
                        positionDrawHistory[cStep].minX,
                        positionDrawHistory[cStep].minY)
            rootObject.requestPaint()
        }
        SettingsPageComponentsSettings.movePadHistory.redoSizeHistory()
    }

    // Function used to save pad to pad list.
    function saveDrawAreaToToolbar()
    {
        var rootObject = canvasContainer.canvasPaintArea
        var catchElement

        ///Prepare pad
        if(SettingsPageComponentsSettings.currentDraggablePad !== null)
            catchElement = _saveCustomizePad()
        else
            catchElement = _saveCustomPadCreated()

        // If the user is not drawing, leave the function.
        if(catchElement === false)
        {
            clearDrawArea("clearAndClose")
            return false
        }

        // Check if image folder exists already.
        qmlCppWrapper._createImagesDirectoryIfNotExist()

        var modelObject = SettingsPageComponentsSettings.m_ShellModificationsComponentPtr
        .draggableElementList.model

        // Searching for duplicate name in current pad elements list.
        if(modelObject.checkIfNameExist(customizePadName) === true)
            customizePadName = SettingsPageComponentsSettings.makeName()


        // Create image path.
        var path = qmlCppWrapper.getApplicationPath()+ "/customPad/"+
                customizePadName.toString()+ ".png";

        canvasContainer.prepareScaledImage()

        console.log("Save to "+ path.toString()+ " Result "+
                    rootObject.save(path.toString()))


        //Add to draggable toolbar list model.
        modelObject.append
                (
                    {
                        /// @note for some odd reason Qt.resolvedUrl() doesn't work here
                        "svgItem": (qmlCppWrapper.resolveUrl(path.toString())),
                        "name":    customizePadName,
                        "m_Width":   1,
                        "m_Height":  1,
                        "basicRotation" :SettingsPageComponentsSettings.currentDraggablePad !== null ?
                                             SettingsPageComponentsSettings.currentDraggablePad.exampleImage.
                                             rotation : 0
                    }
                    )

        _saveNameAndClearArea()
    }


    function useOnlyForThisPad(saveType)
    {
        var catchElement

        if(SettingsPageComponentsSettings.currentDraggablePad !== null)
            catchElement = _saveCustomizePad()
        else
            catchElement = _saveCustomPadCreated()

        if(catchElement === false)
        {
            clearDrawArea("clearAndClose")
            return false
        }

        _saveNameAndClearArea(saveType)

        if(saveType === "startMovePad")
        {
            var nameValue = SettingsPageComponentsSettings.m_ShellModificationsComponentPtr.
            padCreator.getPadName()
            settingsPageManager.shellModificationsState = saveType.toString()
            SettingsPageComponentsSettings.m_ShellModificationsComponentPtr.
            padCreator.setPadName(nameValue)
            if(SettingsPageComponentsSettings.currentDraggablePad != null)
                SettingsPageComponentsSettings.currentDraggablePad.visible = true
        }
    }

    function catchDrawingPad()
    {
        var rootObject = canvasContainer.canvasPaintArea

        var minX = SettingsPageComponentsSettings.drawingAreaDetection.minX
        var minY = SettingsPageComponentsSettings.drawingAreaDetection.minY

        if(rootObject.context === null)
        {
            rootObject.getContext('2d')
        }

        var itemWidth = (SettingsPageComponentsSettings.drawingAreaDetection.maxX - minX)
        var itemHeight =  (SettingsPageComponentsSettings.drawingAreaDetection.maxY - minY)
        var draggablePad = SettingsPageComponentsSettings.currentDraggablePad

        //Maps the drawing area point to the equivalent point within view3d coordinate system
        var imagePos = view3d
        .mapFromItem(rootObject,
                     minX,
                     minY)

        var view3dX = imagePos.x
        var view3dY = imagePos.y

        if(itemWidth === 0
                || itemHeight === 0)
            return false


        rootObject.canvasWindow = Qt.rect(minX,
                                          minY,
                                          itemWidth,
                                          itemHeight)

        rootObject.width = itemWidth
        rootObject.height = itemHeight
        rootObject.requestPaint()

        return [view3dX, view3dY]
    }

    function _saveCustomPadCreated()
    {
        var rootObject = canvasContainer.canvasPaintArea
        var posArray = catchDrawingPad()

        if(posArray === false)
            return false

        var view3dX = posArray[0]
        var view3dY = posArray[1]

        SettingsPageComponentsSettings.createObject(rootObject.toDataURL("image/png", 1),
                                                    customizePadName,
                                                    "doubleClicked",
                                                    rootObject.width,
                                                    rootObject.height,
                                                    view3dX,
                                                    view3dY)

        SettingsPageComponentsSettings.element.opacity = 0
        SettingsPageComponentsSettings.setRecreateItemAsSelect.running = true
    }

    function _saveCustomizePad()
    {
        var canvasRootObject = canvasContainer.canvasPaintArea
        var rootObject = SettingsPageComponentsSettings.currentDraggablePad

        var posArray = catchDrawingPad()

        if(posArray === false)
            return false

        var view3dX = posArray[0]
        var view3dY = posArray[1]

        rootObject.source = canvasContainer.canvasPaintArea.toDataURL("image/png", 1)
        rootObject.exampleImage.height = canvasContainer.canvasPaintArea.height
        rootObject.exampleImage.width = canvasContainer.canvasPaintArea.width
        rootObject.recalculate()
        rootObject.x = view3dX - (rootObject._privateElement.xImagePositionAfterRotation
                                  - rootObject.x)
        rootObject.y = view3dY - (rootObject._privateElement.yImagePositionAfterRotation
                                  - rootObject.y)
        rootObject.opacity = 1

        rootObject.recalculate()
        rootObject.boxSizeRotate()

        SettingsPageComponentsSettings.drawingAreaDetection._reset()
    }

    // Function used to add pad to drawing area.
    function addNewPadToArea(imagePath)
    {
        // Clean up drawing area before drawing the image.
        clearDrawArea("notCleanHistory")
        var rootObject = canvasContainer.canvasPaintArea
        rootObject.requestPaint()

        // 70 px - Basic pad values.
        var itemWidth = 70*canvasContainer.scale.xScale
        var itemHeight = 70*canvasContainer.scale.yScale

        // Catch center of drawing area.
        var xCenterOffset = (rootObject.width/2-itemWidth/2)
        var yCenterOffset = (rootObject.height/2-itemHeight/2)

        var itemX = xCenterOffset
        var itemY = yCenterOffset

        rootObject.context.drawImage(imagePath,
                                     itemX,
                                     itemY,
                                     itemWidth,
                                     itemHeight)

        console.log("Item x "+itemX+" Item y "+itemY+" Item width "+itemWidth+" Item height "+itemHeight)


        // Reset pen values.
        SettingsPageComponentsSettings.drawingAreaDetection.resetValues(itemX+itemWidth,
                                                                        itemY+itemHeight,
                                                                        itemX,
                                                                        itemY
                                                                        )

        rootObject.requestPaint()

        cPush()
    }

    // Function used to add pad to drawing area
    function addPadToArea()
    {
        var rootObject = canvasContainer.canvasPaintArea

        if( rootObject.context === null)
        {
            rootObject.getContext('2d')
        }

        var draggablePad = SettingsPageComponentsSettings.currentDraggablePad.exampleImage
        var itemWidth = draggablePad.width
        var itemHeight = draggablePad.height

        var imageToCanvas = canvasContainer.canvasPaintArea
        .mapFromItem(draggablePad.parent,
                     draggablePad.x,
                     draggablePad.y)

        var itemX = (imageToCanvas.x)
        var itemY = (imageToCanvas.y)

        rootObject.context.drawImage(draggablePad.source,
                                     itemX,
                                     itemY,
                                     itemWidth,
                                     itemHeight)

        SettingsPageComponentsSettings.drawingAreaDetection.resetValues(itemX+itemWidth,
                                                                        itemY+itemHeight,
                                                                        itemX,
                                                                        itemY
                                                                        )


        // Set rotation and center of canvas according to pad settings.
        rootObject.objectRotation.angle = draggablePad.rotation
        rootObject.objectRotation.origin.x = itemX+draggablePad.width/2
        rootObject.objectRotation.origin.y = itemY+draggablePad.height/2

        // Hide pad (from this moment pad is represented as image on canvas).
        SettingsPageComponentsSettings.currentDraggablePad.opacity = 0
        cPush()
    }

    ChooseSideElement {
        id : textContainer

        anchors.horizontalCenter: parent.horizontalCenter

        mouseArea.onClicked : {
            elementClicked()
        }
    }

    Image {
        id: rawImage

        width: parent.width
        height: parent.height
        smooth: true
        visible: false

        anchors {
            top : textContainer.bottom
            topMargin: 30
            horizontalCenter: textContainer.horizontalCenter
        }
        z: view3d.z + 1
        opacity: 0.5
    }

    //Custom qml item to show 3D graphics.
    View3d {
        id : view3d

        anchors {
            top : textContainer.bottom
            topMargin: 30
        }

        z : 99

        width: parent.width
        height: parent.height
        smooth : true

        MouseArea {
            anchors.fill: parent

            onClicked: {
                elementClicked()
            }
        }

        Grid {
            id :drawElementGrid

            visible :  canvasContainer.canvasPaintArea.visible

            rows: (canvasContainer.height)/8
            columns: (canvasContainer.width)/8

            anchors.centerIn: canvasContainer

            Repeater {
                model: drawElementGrid.rows*drawElementGrid.columns

                Rectangle
                {
                    border.color: "white"
                    border.width: 1

                    width: 8; height: 8
                    color: "transparent"
                }
            }
        }

        DrawingArea {
            id : canvasContainer

            z : parent.z
        }
    }

    Rectangle {
        id : imageBorderElement

        color : "transparent"
        width : view3d.width + 30
        height : view3d.height + 30
        radius : 10
        border {
            width : 2
            color : "lightgray"
        }
        anchors.centerIn: view3d
    }


    onCStepChanged:
    {
        if(cStep === 0)
        {
            SettingsPageComponentsSettings.drawingAreaDetection.nothingDraw = true
        }
        else
        {
            SettingsPageComponentsSettings.drawingAreaDetection.nothingDraw = false
        }
    }

    states: [
        State {
            name: "select"
            PropertyChanges { target: textContainer; border.color:  "#407ee2"}
            PropertyChanges { target: imageBorderElement; border.color:  "#407ee2"}
        },
        State {
            name: "unselect"
            PropertyChanges { target: textContainer; border.color:  "transparent"}
            PropertyChanges { target: imageBorderElement; border.color:  "lightgray"}
        }
    ]

}
