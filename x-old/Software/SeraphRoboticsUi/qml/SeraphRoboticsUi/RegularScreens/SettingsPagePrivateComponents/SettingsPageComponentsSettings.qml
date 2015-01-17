pragma Singleton
import QtQuick 2.4

import "SettingsPageComponents"

// Mediator design pattern class.
// Contains settings, ptr
// Class contains pointers and settings for all values which we used in Qt Quick.
QtObject {
    id : rootObject

    //Ptr for current selected side
    //Maybe left or right depends on which direction is currently selected.
    property Item m_CurrentSelectedArea: null

    // Left foot side ptr
    property Item m_LeftSideComponent: null
    // Right foot side ptr
    property Item m_RightSideComponent: null

    // ShellModifications view ptr.
    property Item m_ShellModificationsComponentPtr: null

    // Ptr to main root object - Value used when creating a pad (temporarily as a parent, until the pad is dropped).
    property Item m_MainRootObject: null

    // Value used to store pad in the pad creation process.
    // Should be null when the creation is finished.
    property Item element: null

    /// Value used to store ptr to all pads
    property var draggableObjectArray: []
    property Item currentDraggablePad : null

    /// Values used for storing topcoatSettings.
    property real topcoatThicknessLeft: 6
    property real topcoatThicknessRight: 6
    property real topcoatWeaveLeft: 1
    property real topcoatWeaveRight: 1

    property real canvasPaintPenWidth: 10

    //Value used to store paths in Main3dView.qml (main3dView state).
    property string m_StlFilePath0: "qrc:/exampleImages/right.stl"
    property string m_StlFilePath1: ""


    // Value used to grab all pad images.
    // Used for detecting borders while saving pad properties into xml file.
    // Learn more about this in detectBordersAndSaveToXml() function description.
    property int elementSaved: -1

    /// @Value used to store basic width and height for 3d viewer items.
    // Calculated based on 1280x800 which is our minimal and basic size.
    property real startingSideElementHeight: 539.6
    property real startingSideElementWidth: 285.988


    signal resetSliderValue()

    // Value used to set basic size and position for all created pads.
    property variant setRecreateItemAsSelect: Timer {
        interval: 100
        running: false

        onTriggered: {
            if(draggableObjectArray.length > 1 &&
               settingsPageManager.currentSelectedDirection === "both")
            {
                draggableObjectArray[
                    draggableObjectArray.length-2].setItemAsCurrentItem()
            }
            if(draggableObjectArray.length > 0)
            {
                draggableObjectArray[
                    draggableObjectArray.length-1].setItemAsCurrentItem()
            }
            currentDraggablePad.opacity = 1
            drawingAreaDetection._reset()
            currentDraggablePad.setCurrentPadBasicValues(rootObject)

        }
    }

    // Value used to store pad moving history in ShellModificationsPadCreator.qml
    // After escaping "startMovePad" state we call clearHistory
    property variant movePadHistory : QtObject
    {

        property real movePadHistoryCount: -1
        property var sizePadHistory: []

        function clearHistory()
        {
            console.log("Clear history.")
            sizePadHistory.splice(0, sizePadHistory.length)

            movePadHistoryCount = -1
        }

        function pushIntoSizeHistory(tWidth, tHeight,
                                     tImageBorderX, tImageBorderY,
                                     timageX, timageY,
                                     tImageBorderWidth,
                                     tImageBorderHeight,
                                     tRotation)
        {
            movePadHistoryCount++

            if (movePadHistoryCount < sizePadHistory.length)
                sizePadHistory.length = movePadHistoryCount;

            var size =
                    {
                width: tWidth,
                height: tHeight,
                imageX: timageX,
                imageY: timageY,
                imageBorderX: tImageBorderX,
                imageBorderY: tImageBorderY,
                imageBorderHeight: tImageBorderHeight,
                imageBorderWidth: tImageBorderWidth,
                rotation : tRotation

            };
            sizePadHistory.push(size)
        }

        function undoSizeHistory()
        {
            if(movePadHistoryCount > 0)
            {
                movePadHistoryCount--
                currentDraggablePad.exampleImage.width =
                        sizePadHistory[movePadHistoryCount].width*currentDraggablePad.elementScale.xScale
                currentDraggablePad.exampleImage.height =
                        sizePadHistory[movePadHistoryCount].height*currentDraggablePad.elementScale.yScale

                currentDraggablePad.exampleImage.x =
                        sizePadHistory[movePadHistoryCount].imageX
                currentDraggablePad.exampleImage.y =
                        sizePadHistory[movePadHistoryCount].imageY

                currentDraggablePad.x =
                        sizePadHistory[movePadHistoryCount].imageBorderX*currentDraggablePad.elementScale.xScale
                currentDraggablePad.y =
                        sizePadHistory[movePadHistoryCount].imageBorderY*currentDraggablePad.elementScale.yScale

                currentDraggablePad.width =
                        sizePadHistory[movePadHistoryCount].imageBorderWidth
                currentDraggablePad.height =
                        sizePadHistory[movePadHistoryCount].imageBorderHeight
                currentDraggablePad.exampleImage.rotation = sizePadHistory[movePadHistoryCount]
                .rotation


            }
        }

        function redoSizeHistory()
        {
            if (movePadHistoryCount < sizePadHistory.length-1)
            {
                movePadHistoryCount++
                currentDraggablePad.exampleImage.width =
                        sizePadHistory[movePadHistoryCount].width*currentDraggablePad.elementScale.xScale
                currentDraggablePad.exampleImage.height =
                        sizePadHistory[movePadHistoryCount].height*currentDraggablePad.elementScale.yScale

                currentDraggablePad.exampleImage.x =
                        sizePadHistory[movePadHistoryCount].imageX
                currentDraggablePad.exampleImage.y =
                        sizePadHistory[movePadHistoryCount].imageY

                currentDraggablePad.x =
                        sizePadHistory[movePadHistoryCount].imageBorderX*currentDraggablePad.elementScale.xScale
                currentDraggablePad.y =
                        sizePadHistory[movePadHistoryCount].imageBorderY*currentDraggablePad.elementScale.yScale

                currentDraggablePad.width =
                        sizePadHistory[movePadHistoryCount].imageBorderWidth
                currentDraggablePad.height =
                        sizePadHistory[movePadHistoryCount].imageBorderHeight
                currentDraggablePad.exampleImage.rotation = sizePadHistory[movePadHistoryCount]
                .rotation

            }
        }
    }

    /// @note Object used to store values which we use during pad recreation process.
    property variant recreateAttributes: QtObject {
        property int xValue
        property int yValue
        property string side
    }

    // Values used to store settings for currently dragged pad in case we need to restore them
    property variant basicDraggableElementValues: QtObject {
        property real imageRotation
        property real imageWidth
        property real imageHeight
    }

    // Values used to store coordinates for drawing a pad.
    // Updated always when we painted something inside canvas area.
    // @param nothingDraw - Value used to hide undo and "stop drawing move pad" button when customizing
    // or creating a pad.
    property variant drawingAreaDetection: QtObject {
        property int maxY : 0
        property int maxX : 0
        property int minY:  0
        property int minX : 0

        property bool nothingDraw : true

        function setValue(inputValueX, inputValueY, canvasContainer)
        {
            var rootObject = canvasContainer.canvasPaintArea

            if(inputValueX < 0 || inputValueX > rootObject.width || inputValueY < 0 ||
                    inputValueY > rootObject.height)
                return

            console.log("Got "+inputValueX+" y "+inputValueY)

            if(minX === 0)
                minX = inputValueX-canvasPaintPenWidth
            if(minY === 0)
                minY = inputValueY-canvasPaintPenWidth

            if(inputValueX > maxX)
            {
                maxX = inputValueX+canvasPaintPenWidth
            }
            else if(inputValueX < minX)
            {
                minX = inputValueX-canvasPaintPenWidth
            }

            if(inputValueY > maxY)
            {
                maxY = inputValueY+canvasPaintPenWidth
            }
            else if(inputValueY < minY)
            {
                minY = inputValueY-canvasPaintPenWidth
            }
        }

        function resetValues(tMaxX, tMaxY, tMinX, tMinY)
        {
            maxY = tMaxY
            maxX = tMaxX
            minY = tMinY
            minX = tMinX
        }

        function _reset()
        {
            maxY = 0
            maxX = 0
            minX = 0
            minY = 0
        }
    }

    function setTopcoatThickness(value, applyToBoth, weaveValue)
    {
        if(settingsPageManager.currentSelectedDirection === "left"
                || settingsPageManager.currentSelectedDirection === "both"
                || applyToBoth === true)
        {
            console.log("Set left"+value)
            topcoatThicknessLeft = value
            topcoatWeaveLeft = weaveValue
        }
        if(settingsPageManager.currentSelectedDirection === "right"
                || settingsPageManager.currentSelectedDirection === "both"
                || applyToBoth === true)
        {
            console.log("Set right"+value)
            topcoatThicknessRight = value
            topcoatWeaveRight = weaveValue
        }
    }

    // Function used to grab images in javascript.
    // We need to wrap async function grabToImage() into sync function.
    // To make this work we use elementSaved as our counter
    // which  we increase after each sucesfully grabbed image.
    // Here we only catch first element and then logic skip under
    // onElementSavedChanged signal handler.
    function detectBordersAndSaveToXml()
    {
        // nothing to do when nothing is in pad array
        if(draggableObjectArray.length === 0)
            return

        /// @note reset counter and got first value
        rootObject.elementSaved = 0

        var element = draggableObjectArray[rootObject.elementSaved]
        var elementImage = element
        var imageHeight = elementImage.height
        var imageWidth = elementImage.width

        //// @note grab first value and start  increase elementSaved value
        /// next elements application catching in respective onElementSavedChanged
        /// place.
        console.log("Grab Image result: "+elementImage.grabToImage(function(result) {
            qmlCppWrapper.appendElementToShellList(element.direction,
                                                   element.m_Name,
                                                   element.m_Depth,
                                                   element.m_Height,
                                                   element.m_Stiffness,
                                                   result.image,
                                                   element.realX,
                                                   element.realY)
            rootObject.elementSaved++
        }));

    }

    function switchCurrentDirectionStateToCurrentUsableArea()
    {
        if(m_CurrentSelectedArea.objectName === "leftSide")
            settingsPageManager.currentSelectedDirection = "left"
        else if(m_CurrentSelectedArea.objectName === "rightSide")
            settingsPageManager.currentSelectedDirection = "right"
    }

    function deleteElementFromArrayOnElementDestroy(mainWindow)
    {
        unselectCurrentDraggablePad()
        console.log("Array length before delete: "
                    +draggableObjectArray.length)
        var index = draggableObjectArray.indexOf(mainWindow)

        if (index > -1)
        {
            draggableObjectArray.splice(index, 1);
        }
        console.log("Delete item from index "+index+" Current length: "
                    +draggableObjectArray.length)
    }

    // Function used to hide all pads. Used for posting or topcoat shell modifications state.
    function hideDraggableElements()
    {
        console.log("Hide draggable")
        for(var i=0; i<draggableObjectArray.length; i++)
        {
            draggableObjectArray[i].visible = false
        }
    }

    //Function used to show all pads. Used for elementSettings shell modifications state.
    function showDraggableElements()
    {
        console.log("Show draggable")
        for(var i=0; i<draggableObjectArray.length; i++)
        {
            draggableObjectArray[i].visible = true
        }
    }

    function checkIfValuesTheSame()
    {
        var value = (topcoatThicknessLeft ===
                     topcoatThicknessRight
                     &&
                     topcoatWeaveLeft ===
                     topcoatWeaveRight
                     )
        return value
    }

    ///Function used to set labels position.
    function setLabels()
    {
        var lastLeftElementY = 0
        var lastRightElementY = 0
        var lastLeftElementX = 1
        var lastRightElementX = 1
        var heightOffset = 130
        var elementHeightMargin = 50
        var maxColumn = 2
        var rightLabelsColumnMargin = 35
        var leftLabelsColumnMargins = 20
        var logoOffset = m_MainRootObject.logoHeight+5

        var middleHeightOffset = 200
        var lastMiddleYPos = 0
        var middleY = 0
        var maxPos = m_RightSideComponent.height - 50


        console.log()

        //Sort pads by y position in ascending order.
        draggableObjectArray.sort(
                    function sortArrayViaYPos(first, second)
                    {
                        return first.y - second.y
                    }
                    )


        for(var i=0; i<draggableObjectArray.length; i++)
        {
            console.log("Current element y: "+draggableObjectArray[i].y)

            var objectCenter = draggableObjectArray[i].width/2 // Value used to store pad center position.
            var newPosObj // Value used to store new label position.
            var tempElement = draggableObjectArray[i].typeNameText // Current label.
            var heightDiffrent = tempElement.height+elementHeightMargin // Label height.
            var direction = draggableObjectArray[i].direction // pad parent (left, right foot).
            var textElementWithMarginsWidth = tempElement.width+20 //20 px - Text left,right margings.
            var bothButtonHeight = 50 // Value used to store 'Both' button height. The area between the foot should
            //  be filled with labels starting from button's bottom position.


            // Calculate left margin of the left foot (right margin of right foot will be the same) and
            // then divid it by label's width (textElementWithMarginsWidth)
            maxColumn = parseInt((m_MainRootObject.
                                  mapFromItem(m_LeftSideComponent.parent,m_LeftSideComponent.x,
                                              m_LeftSideComponent.y).x/textElementWithMarginsWidth)-0.25)

            console.log("Column count "+maxColumn)


            // Calculate middle area width.
            var centerAreaWidth = (m_RightSideComponent.x -
                          m_LeftSideComponent.x -
                          m_LeftSideComponent.width + (textElementWithMarginsWidth-20))/2

            // Increase left column.
            if(lastLeftElementY > m_LeftSideComponent.height+heightOffset/2)
            {
                lastLeftElementY = 0
                lastLeftElementX += 1
            }

            //Check if the label doesn't oberlap logo button.
            else if(lastRightElementX == maxColumn  &&
                    lastRightElementY > m_RightSideComponent.height+heightOffset/2-
                    logoOffset)
            {
                lastRightElementY = 0
                lastRightElementX += 1
            }
            // Increase right column.
            else if(lastRightElementY > m_RightSideComponent.height+heightOffset/2)
            {
                lastRightElementY = 0
                lastRightElementX += 1
            }

            if(lastLeftElementX > maxColumn)
            {
                lastLeftElementX = 1
            }

            if(lastRightElementX > maxColumn)
            {
                lastRightElementX = 1
            }

            if(direction === "right")
            {
                // Check if label should be placed between the foot
                if(draggableObjectArray[i].x+objectCenter <= m_RightSideComponent.width/2 &&
                        lastMiddleYPos < m_RightSideComponent.height-70)
                {
                    lastMiddleYPos += heightDiffrent
                    middleY += 1
                    draggableObjectArray[i].labelDirection = "left"
                    newPosObj = draggableObjectArray[i].typeNameText.parent.
                    mapFromItem(m_MainRootObject,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).x-centerAreaWidth
                                ,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).y-heightOffset+bothButtonHeight
                                +lastMiddleYPos)
                }
                else
                {
                    lastRightElementY += heightDiffrent
                    draggableObjectArray[i].labelDirection = "right"
                    newPosObj = draggableObjectArray[i].typeNameText.parent.
                    mapFromItem(m_MainRootObject,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).x
                                +m_RightSideComponent.width+rightLabelsColumnMargin
                                +(lastRightElementX-1)*textElementWithMarginsWidth,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).y
                                +lastRightElementY-heightOffset)
                }
            }
            else if(direction === "left")
            {
                // Check if label should be placed between the foot
                if(draggableObjectArray[i].x+objectCenter >= m_LeftSideComponent.width/2 &&
                        lastMiddleYPos < m_LeftSideComponent.height-70)
                {
                    lastMiddleYPos += heightDiffrent
                    middleY += 1
                    draggableObjectArray[i].labelDirection = "right"

                    newPosObj = draggableObjectArray[i].typeNameText.parent.
                    mapFromItem(m_MainRootObject,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).x
                                -centerAreaWidth ,
                                m_MainRootObject.mapFromItem(m_RightSideComponent).y-
                                heightOffset+bothButtonHeight
                                +lastMiddleYPos)
                }
                else
                {
                    lastLeftElementY += heightDiffrent
                    draggableObjectArray[i].labelDirection = "left"
                    newPosObj = draggableObjectArray[i].typeNameText.parent.
                    mapFromItem(m_MainRootObject,
                                m_MainRootObject.mapFromItem(m_LeftSideComponent).x
                                -lastLeftElementX*textElementWithMarginsWidth-leftLabelsColumnMargins,
                                m_MainRootObject.mapFromItem(m_LeftSideComponent).y
                                +lastLeftElementY-heightOffset
                                )
                }
            }

            tempElement.x = newPosObj.x
            tempElement.y = newPosObj.y
        }
    }

    //Function used to generate random name for newly created pad.
    function makeName()
    {
        var text = "";
        var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

        for(var i=0; i < 5; i++)
            text += possible.charAt(Math.floor(Math.random() * possible.length));

        return text;
    }

    // Function used to unselect currently selected item.
    function unselectCurrentDraggablePad()
    {
        if(currentDraggablePad !== null)
            currentDraggablePad.opacity = 1
        currentDraggablePad = null

        if(settingsPageManager === null)
            return


        if(settingsPageManager.shellModificationsState === "elementSettings"
                || settingsPageManager.shellModificationsState == "startMovePad"
                )
        {
            settingsPageManager.shellModificationsState = "down"
        }
    }

    function saveValues(stiffnes,mmValues, leftX, rightX,
                        depthValue)
    {
        console.log("Save Values "+leftX+" right "+rightX+" depth "+depthValue+" height "+
                    (parseInt(mmValues)-depthValue))
        currentDraggablePad.m_Stiffness = stiffnes
        currentDraggablePad.m_Height = parseInt(mmValues)-depthValue
        currentDraggablePad.m_HeightDepthXL = leftX
        currentDraggablePad.m_HeightDepthXR = rightX
        currentDraggablePad.m_Depth = depthValue
    }

    function createObject(svgItem, typeName, createBy, width, height, x, y, basicRotation)
    {
        currentDraggablePad = null
        var selectedArea = settingsPageManager.currentSelectedDirection

        if(selectedArea == "both" &&
                createBy.toString() === "oneClicked")
        {
            _allocateObject(svgItem, "both", typeName, createBy, width, height, x, y ,basicRotation);
            return;
        }

        if(createBy === "recreate")
        {
            _allocateObject(svgItem, recreateAttributes.side, typeName, createBy, width, height, x, y, basicRotation);
            return
        }

        if(selectedArea == "right")
        {
            _allocateObject(svgItem, "right", typeName, createBy, width, height, x, y, basicRotation)
        }

        if(selectedArea == "left")
        {
            _allocateObject(svgItem, "left", typeName, createBy, width, height, x, y, basicRotation);
        }

        if(selectedArea == "both")
        {
            m_CurrentSelectedArea = m_RightSideComponent
            _allocateObject(svgItem, "right", typeName, createBy, width, height, x, y, basicRotation)
            m_CurrentSelectedArea = m_LeftSideComponent
            _allocateObject(svgItem, "left", typeName, createBy, width, height, x, y, basicRotation)
        }
    }

    // Function used to recreate pad element. Checks if pad is dropped onto selected foot.
    // If yes, then it recreates pad with selected foot as parent. If no, destroys the pad.
    function recreateElement(mouseX, mouseY, parentItem)
    {
        console.log("direction"+element.direction)
        if(element.direction === "both")
        {
            var imgPath = element.source
            var typeName = element.m_Name
            var elementHeight = element.height
            var elementWidth = element.width
            var elementX = element.x
            var elementY = element.y

            if( element._privateElement._checkIfDropInside(m_LeftSideComponent))
            {
                recreateAttributes.side = "left"
                m_CurrentSelectedArea = m_LeftSideComponent
                console.log("under left element")
                setRecreateItemAsSelect.running = true
            }
            else if( element._privateElement._checkIfDropInside(m_RightSideComponent))
            {
                recreateAttributes.side = "right"
                m_CurrentSelectedArea = m_RightSideComponent
                console.log("under right element")
                setRecreateItemAsSelect.running = true
            }

            recreateAttributes.xValue = parentItem.mapToItem
                    (m_CurrentSelectedArea.view3d,
                     mouseX,
                     mouseY).x

            recreateAttributes.yValue = parentItem.mapToItem
                    (m_CurrentSelectedArea.view3d,
                     mouseX,
                     mouseY).y

            console.log("X value "+recreateAttributes.xValue+" Y value "+recreateAttributes.yValue)

            element.destroy()
            element =  null
            createObject(imgPath,
                         typeName,
                         "recreate")
        }
    }

    function _allocateObject(svgItem, side, typeName, createBy, width, height, x, y, basicRotation)
    {
        var newObject = Qt.
        createComponent("SettingsPageComponents/DraggableElementComponents/DraggableElement.qml")

        if (newObject.status === Component.Ready)
        {
            var parentElement = m_CurrentSelectedArea.view3d

            if(side === "both")
                parentElement = m_MainRootObject

            var draggableElement = newObject.createObject(parentElement);
            draggableElement.source = svgItem;
            draggableElement.m_Name = typeName
            draggableElement.direction = side

            if(typeof (width) !== "undefined"  && typeof (height) !== "undefined")
            {
                draggableElement.exampleImage.width = width
                draggableElement.exampleImage.height = height
            }

            if(createBy === "recreate")
            {
                console.log("Recreate x "+recreateAttributes.xValue
                            +"Recreate y "+ recreateAttributes.yValue)
                if(typeof (x) !== "undefined"  && typeof (y) !== "undefined")
                {
                    draggableElement.x = x
                    draggableElement.y = y
                }

                else
                {
                    draggableElement.x = recreateAttributes.xValue
                    draggableElement.y = recreateAttributes.yValue
                }
            }
            else
            {
                if(typeof (x) !== "undefined"  && typeof (y) !== "undefined")
                {
                    console.log("create"+draggableElement.exampleImage.x+" y "+draggableElement.exampleImage.y)
                    draggableElement.x = x-draggableElement.minValue/2
                    draggableElement.y = y-draggableElement.minValue/2
                }
                else
                {
                    draggableElement.x = parentElement.width/2 - draggableElement.width/2
                    draggableElement.y = parentElement.height/2 - draggableElement.height/2
                }
            }

            if(typeof (basicRotation) !== "undefined" )
            {
                draggableElement.exampleImage.rotation = basicRotation
            }

            element = draggableElement
            draggableObjectArray.push(element)
        }
    }


    onElementSavedChanged:
    {
        // Ignore first value becauase it was already grabbed into detectBordersAndSaveToXml()
        if(elementSaved === 0)
        return;

        // All images grabbed, leave the function.
        if(elementSaved === draggableObjectArray.length)
        {
            console.log("Saved all borders, save result to xml files.")
            qmlCppWrapper.saveShellModifications()
            return;
        }

        //Grab next image.
        if( elementSaved < draggableObjectArray.length && elementSaved != -1)
        {
            var element = draggableObjectArray[elementSaved]
            var elementImage = element
            var imageHeight = elementImage.height
            var imageWidth = elementImage.width

            console.log("Grab Image result: "+elementImage.grabToImage(function(result) {
                qmlCppWrapper.appendElementToShellList(element.direction,
                                                       element.m_Name,
                                                       element.m_Depth,
                                                       element.m_Height,
                                                       element.m_Stiffness,
                                                       result.image,
                                                       element.realX,
                                                       element.realY)
                rootObject.elementSaved++
            }));
        }
        else // Reset the counter.
        {
            elementSaved = -1
        }
    }

    Component.onDestruction:
    {
        console.log("Destroy draggable array component.
                     Clear draggable elements array,array length: "
                    +draggableObjectArray.length)

        for(var i = 0; i<draggableObjectArray.length; i++)
        {
            draggableObjectArray[i].destroy()
        }
    }
}
