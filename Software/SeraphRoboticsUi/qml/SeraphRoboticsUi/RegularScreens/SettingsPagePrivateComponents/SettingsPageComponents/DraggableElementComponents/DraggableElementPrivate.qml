import QtQuick 2.4

import "../../../../Components"
import "../DraggableElementComponents" 1.0
import "../../"

QtObject {
    property Item currentElement: null

    // Value used for catching image position after rotation.
    property real xImagePositionAfterRotation
    property real yImagePositionAfterRotation

    // Function used to recalculate bounding box.
    function _recalculate()
    {
        var root = currentElement.parent
        var imageReference = exampleImage
        var boundingRect = currentElement.boundingRect

        xImagePositionAfterRotation = imageReference.mapToItem(root, 0, 0).x
        yImagePositionAfterRotation = imageReference.mapToItem(root, 0, 0).y

        var x2 = imageReference.mapToItem(root, imageReference.width, 0).x
        var y2 = imageReference.mapToItem(root, imageReference.width, 0).y

        var x3 = imageReference.mapToItem(root, imageReference.width, imageReference.height).x
        var y3 = imageReference.mapToItem(root, imageReference.width, imageReference.height).y

        var x4 = imageReference.mapToItem(root, 0, imageReference.height).x
        var y4 = imageReference.mapToItem(root, 0, imageReference.height).y

        var min_x = Math.min(xImagePositionAfterRotation, x2, x3, x4)
        var min_y = Math.min(yImagePositionAfterRotation, y2, y3, y4)

        var max_x = Math.max(xImagePositionAfterRotation, x2, x3, x4) - boundingRect.x
        var max_y = Math.max(yImagePositionAfterRotation, y2, y3, y4) - boundingRect.y

        boundingRect.x = min_x
        boundingRect.y = min_y
        boundingRect.width = max_x
        boundingRect.height = max_y

    }

    // Function used to check if pad was dropped within foot area.
    function _checkDropAreas()
    {
        var dropArea = 0;

        var leftSide = SettingsPageComponentsSettings.m_LeftSideComponent
        var rightSide = SettingsPageComponentsSettings.m_RightSideComponent
        var mainWindow = currentElement

        /// @note no one selected
        if(leftSide.state === "unselect"
                && rightSide.state === "unselect")
            return

        ///@note both selected
        if(leftSide.state === "select"
                && rightSide.state === "select") {
            var tempLeftSide = _checkIfDropInside(leftSide)
            var tempRightSide = _checkIfDropInside(rightSide)

            if( tempLeftSide === false && tempRightSide === false)
                mainWindow.destroy()
            return
        }

        if(leftSide.state === "select")
            dropArea = leftSide
        else if(rightSide.state === "select")
            dropArea = rightSide


        if(_checkIfDropInside(dropArea) === false)
            mainWindow.destroy()
    }

    // Function used as a helper for _checkDropAreas; returns true if Pad was dropped inside foot area,
    // otherwise returns false.
    function _checkIfDropInside(checkedDropArea)
    {
        var boundingRect = currentElement

        var catchElement = checkedDropArea.view3d.mapFromItem(boundingRect.parent,
                                                       boundingRect.x,
                                                       boundingRect.y)
        var xPosition = catchElement.x
        var yPosition = catchElement.y

        var widthSensitiveArea = (boundingRect.width/2)*currentElement.elementScale.xScale
        var heightSensitiveArea = (boundingRect.height/2)*currentElement.elementScale.yScale
        var SideXMin =
                Math.max(-widthSensitiveArea, -100)
        var SideYMin =
                Math.max(-heightSensitiveArea, -100)

        console.log("Element width "+boundingRect.width+" view 3d width "+checkedDropArea.view3d.width)

        var SideXMax = checkedDropArea.view3d.width
                -boundingRect.width*currentElement.elementScale.xScale+widthSensitiveArea

        var SideYMax = checkedDropArea.view3d.height
                -boundingRect.height*currentElement.elementScale.yScale+heightSensitiveArea

        console.log("Current X"+xPosition+"Current Y "+yPosition+" min X "+SideXMin+" max X"+
                    SideXMax+" min y"+SideYMin+" max y "+SideYMax)

        if (yPosition >= SideYMin
                && yPosition<= SideYMax
                && xPosition >= SideXMin
                && xPosition <= SideXMax
                )
            return true
        else
            return false
    }
}
