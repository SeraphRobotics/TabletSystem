import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

import "../../../Components"
import "../" 1.0

Item {
    id : sliderContainer

    height : 5
    width : 312

    property real value: 1
    property real stepSize: (mainLine.width)/24
    property alias rightTriangle: rightTriangle
    property alias leftTriangle: leftTriangle
    property alias border: border

    property int tickmarkPos: 14
    property int depthValue: ((hashLine.x - leftTriangle.x)/stepSize)-0.5
    property int heightValue: parseInt((border.width/stepSize)+1)

    property int m_BasicLeftTriangleX : 80
    property int m_BasicRightTriangleX : stepSize*(tickmarkPos+1)

    Rectangle {
        id : mainLine

        height : 2
        color : "#666666"

        anchors {
            left : minusButton.right
            leftMargin: 15
            right : plusButton.left
            rightMargin: 15
        }
    }

    Rectangle {
        id : border

        color : "#666666"
        height : 3
        y: -leftTriangle.height

        anchors {
            left : leftTriangle.right
            leftMargin: -3
            right : rightTriangle.left
            rightMargin: -3
        }

        MouseArea {
            id : mouse

            anchors
            {
                verticalCenter: parent.verticalCenter
                left : parent.left
                leftMargin: 5
                right : parent.right
                rightMargin: 5
            }

            height : leftTriangle.height+border.height+10

            drag.target : border
            drag.axis: Drag.XAxis
            onPressed: {
                border.anchors.left = undefined
                border.anchors.right = undefined
                leftTriangle.anchors.right = border.left
                leftTriangle.anchors.rightMargin = -3
                rightTriangle.anchors.left = border.right
                rightTriangle.anchors.leftMargin = -3
            }

            onReleased: {
                border.anchors.left = leftTriangle.right
                border.anchors.right = rightTriangle.left
                leftTriangle.anchors.right = undefined
                rightTriangle.anchors.left = undefined
                border.anchors.leftMargin = -3
                border.anchors.rightMargin = -3
            }

            Rectangle {
                color : "transparent"
                anchors.fill: parent
            }
        }

        onXChanged:
        {
            var widthOffest = leftTriangle.width/2-3
            var rightMax = hashLine.x+widthOffest
            var leftMin =  mainLine.x+widthOffest
            var rightMin = hashLine.x-widthOffest

            var rightMaxWidthConditional = mainLine.x+mainLine.width-widthOffest
            var x1 = x+border.width

            if(x <= leftMin
                    && mouse.pressed === true)
            {
                x = leftMin
            }
            else if(x > rightMax && rightMax > widthOffest)
            {
                x = rightMax
            }
            else if(x1 > rightMaxWidthConditional && rightMaxWidthConditional > 0 && x1 > 0)
            {
                x = mainLine.width-border.width+mainLine.x-widthOffest
            }

            if(x+border.width < rightMin && mouse.pressed === true)
            {
                x = rightMin-border.width
            }
        }
    }

    Image {
        id : leftTriangle

        source : "qrc:/QmlResources/pointer.png"
        y: -leftTriangle.height
        x: m_BasicLeftTriangleX
        z : 99

        MouseArea {
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: -3
            }

            width : leftTriangle.width+6
            height : leftTriangle.height+5
            drag.target : leftTriangle
            drag.axis: Drag.XAxis

            Rectangle {
                color : "transparent"
                anchors.fill: parent
            }
        }

        onXChanged:
        {
            var widthOffest = leftTriangle.width/2
            var rightMax = hashLine.x-widthOffest
            var leftMax =  mainLine.x-widthOffest
            var rightTrianglePos = rightTriangle.x-leftTriangle.width

            if(x <leftMax)
                x = leftMax
            else if(x > rightTrianglePos)
                x = rightTrianglePos
            else if(x > rightMax)
                x = rightMax
        }
    }

    Image {
        id : rightTriangle

        source : "qrc:/QmlResources/pointer.png"
        y : -leftTriangle.height
        x : m_BasicRightTriangleX

        MouseArea {
            anchors.centerIn: parent
            anchors {
                verticalCenter: parent.verticalCenter
                horizontalCenter: parent.horizontalCenter
                horizontalCenterOffset: 3
            }

            width : leftTriangle.width+6
            height : leftTriangle.height+5

            drag.target : rightTriangle
            drag.axis: Drag.XAxis

            Rectangle {
                color : "transparent"
                anchors.fill: parent
            }
        }

        onXChanged:
        {
            var widthOffest = rightTriangle.width/2
            var rightMax = mainLine.x+mainLine.width-widthOffest
            var leftMax =  mainLine.x-widthOffest
            var leftTrianglePos = leftTriangle.x+rightTriangle.width
            var rightMin = hashLine.x-widthOffest

            if(x < rightMin)
            {
                x = rightMin
            }
            else if(x < leftTrianglePos && x != leftTrianglePos)
            {
                x = leftTrianglePos
            }
            else if(x > rightMax && rightMax > 0)
            {
                x = rightMax
            }
        }
    }

    GrayDescriptionText {
        font.pixelSize : 11

        text: "within shell"

        anchors {
            left : mainLine.left
            top : mainLine.bottom
            topMargin: 5
        }
    }
    GrayDescriptionText {
        font.pixelSize : 11

        text: "above shell"

        anchors {
            right : mainLine.right
            top : mainLine.bottom
            topMargin: 5
        }
    }

    Rectangle {
        id : hashLine

        height: 15
        width: 1
        color : "#666666"

        anchors{
            left: mainLine.left
            leftMargin: tickmarkPos*stepSize
            top : mainLine.bottom
        }

    }

    ImageBasedButton {
        id : minusButton

        anchors {
            left : parent.left
            verticalCenter: parent.verticalCenter
        }
        source : "qrc:/QmlResources/btn-minus.png"

        onCustomClicked: {
                leftTriangle.x -= stepSize
        }
    }

    ImageBasedButton {
        id : plusButton

        anchors {
            right : parent.right
            verticalCenter: parent.verticalCenter
        }
        source : "qrc:/QmlResources/btn-plus.png"

        onCustomClicked: {
            rightTriangle.x += stepSize
        }
    }

    onHeightValueChanged: {
         value = depthValue+heightValue
    }

    onDepthValueChanged: {
        value = depthValue+heightValue
    }

}
