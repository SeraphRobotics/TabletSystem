// EdgeEditor.qml

import QtQuick 2.2

import "drawing.js" as Drawing

Item {
    id: root

    //// @note in future we need something automatic to edge detection like openCv.

    property alias imageSource: img.source
    property alias canvas : canvas
    property int initWidth
    property int initHeight
    property int edgeRadius: 25
    property real scaleFactor: 1.0

    // Constant points in format [x1, y1, x2, y2, ...]
    //   (need to set from outside by setConstantPoints)
    property var constantPoints: []

    // Centers of edges in format [edge1_x, edge1_y, edge2_x, edge2_y, ...]
    //   needed to initialize edges positions
    //   (set from outside by setInitEdgePoints)
    property var edgePoints: []

    function setLeftEdge()
    {
        var constantPoints = [
                    0.91, 0.37,
                    0.74, 0.54,
                    0.87, 0.76,
                    0.81, 0.91,
                    0.61, 0.97,
                    0.46, 0.94,
                    0.35, 0.83,
                    0.23, 0.67,
                    0.16, 0.54,
                    0.14, 0.47,
                    0.12, 0.41  ]

        var initEdgePoints = [
                    0.12, 0.22,
                    0.27, 0.10,
                    0.45, 0.06,
                    0.85, 0.09,]

        setConstantPointsF(constantPoints)
        setInitEdgePointsF(initEdgePoints)
    }

    function setRightEdge()
    {
        var constantPoints = [
                    0.91, 0.37,
                    0.84, 0.54,
                    0.71, 0.76,
                    0.61, 0.91,
                    0.43, 0.97,
                    0.26, 0.94,
                    0.15, 0.83,
                    0.23, 0.67,
                    0.26, 0.54,
                    0.24, 0.47,
                    0.12, 0.41 ]

        var initEdgePoints = [
                    0.08, 0.09,
                    0.45, 0.05,
                    0.73, 0.09,
                    0.90, 0.14,]

        setConstantPointsF(constantPoints)
        setInitEdgePointsF(initEdgePoints)
    }

    function setConstantPoints(newConstantPoints) {
        if (newConstantPoints.length%2 !== 0) {
            console.log("EdgeEditor. Wrong input array for setConstantPoints.")
            return;
        }
        constantPoints = newConstantPoints
    }

    function setInitEdgePoints(newInitEdgePoints) {
        if (newInitEdgePoints.length%2 !== 0) {
            console.log("EdgeEditor. Wrong input array for setInitEdgePoints.")
            return;
        }
        edgePoints = newInitEdgePoints
    }

    // Param newConstantsPointsF is array of [x1, y1, x2, y2, ...] where 0 <= xi, yi <= 1.0,
    //   and each pair xi, yi represents point coords on image img
    function setConstantPointsF(newConstantsPointsF) {
        if (img.source != "") {

            if (newConstantsPointsF.length %2 !== 0) {
                console.log("EdgeEditor. Wrong input array for setConstantPointsF.")
                return;
            }

            constantPoints = []
            for (var i = 0; i < newConstantsPointsF.length; i+=2) {
                constantPoints.push( newConstantsPointsF[i] * canvas.imgWidth )
                constantPoints.push( newConstantsPointsF[i+1] * canvas.imgHeight )
            }
        } else {
            console.log("EdgeEditor. img.source is empty!")
        }
    }

    // Param newInitEdgePointsF is array of [x1, y1, x2, y2, ...] where 0 <= xi, yi <= 1.0,
    //   and each pair xi, yi represents point coords on image img
    function setInitEdgePointsF(newInitEdgePointsF) {
        if (img.source != "") {

            if (newInitEdgePointsF.length%2 !== 0) {
                console.log("EdgeEditor. Wrong input array for setInitEdgePointsF.")
                return;
            }

            edgePoints = []
            for (var i = 0; i < newInitEdgePointsF.length; i+=2) {
                edgePoints.push( newInitEdgePointsF[i] * canvas.imgWidth )
                edgePoints.push( newInitEdgePointsF[i+1] * canvas.imgHeight )
            }
        } else {
            console.log("EdgeEditor. img.source is empty!")
        }
    }

    function checkEdgeClicked(edgeNnum, x, y) {
        var pointX = edgePoints[edgeNnum*2] * scaleFactor
        var pointY = edgePoints[edgeNnum*2 + 1] * scaleFactor
        var xs = (x - pointX)
        var ys = (y - pointY)
        return (Math.sqrt(xs*xs + ys*ys) < edgeRadius)
    }

    function detectClickedEdge(x, y) {
        for (var i = 0; i < edgePoints.length; i+=2) {
            var edgeNumber = i/2
            if (checkEdgeClicked(edgeNumber, x, y)) {
                return edgeNumber
            }
        }
        return -1
    }

    clip: true

    Image { id: img; visible: false; }

    MouseArea {
        id: mouseArea
        property int currentEdgeNum: -1
        anchors.fill: parent

        onPressed: {
            currentEdgeNum = parent.detectClickedEdge(mouseX, mouseY)
            canvas.requestPaint()
        }

        onMouseXChanged: {
            if (mouseX < 0 || mouseX > parent.width) return;
            if (mouseArea.pressed && currentEdgeNum !== -1) {
                edgePoints[currentEdgeNum*2] = Math.round(mouseX / scaleFactor)
            }
            canvas.requestPaint()
        }

        onMouseYChanged: {
            if (mouseY < 0 || mouseY > parent.height) return;
            if (mouseArea.pressed && currentEdgeNum !== -1) {
                edgePoints[currentEdgeNum*2 + 1] = Math.round(mouseY / scaleFactor)
            }
            canvas.requestPaint()
        }

        onReleased: {
            currentEdgeNum = -1
        }

        onClicked: {
            console.log("Edge coords: ")
            for (var i = 0; i < edgePoints.length; i+=2) {
                if (i%2==0) console.log("№ ", i/2);
                console.log("X (in pixels)\t",
                            edgePoints[i].toFixed(2), "\t(absolute, % width)",
                            (edgePoints[i]/canvas.imgWidth).toFixed(2))
                console.log("Y (in pixels)\t",
                            edgePoints[i+1].toFixed(2), "\t(absolute, % height)",
                            (edgePoints[i+1]/canvas.imgHeight).toFixed(2))
            }
        }
    }


    Canvas {
        id: canvas

        width: parent.width
        height: parent.height
        contextType: "2d"

        property real imgScaleFactorToFitHeight: canvas.height / img.height
        property real imgWidth: img.width*imgScaleFactorToFitHeight/scaleFactor
        property real imgHeight: canvas.height/scaleFactor


        onPaint: {
            context.save()
            context.scale(scaleFactor, scaleFactor)
            context.clearRect(0, 0, width, height)

            context.drawImage(img, 0, 0, imgWidth, imgHeight)

            context.strokeStyle = "darkblue"
            context.lineWidth = 5
            context.lineJoin = "round"
            var allPoints = edgePoints.concat(constantPoints)

            context.beginPath()
            Drawing.drawSpline(context, allPoints, 0.5)
            context.closePath()
            context.fillStyle = "rgba(0, 0, 120, 0.4)"
            context.fill()
            var i;
            for (i = 0; i < edgePoints.length; i+=2) {
                Drawing.drawEdge(context, edgePoints[i], edgePoints[i+1], edgeRadius)
            }
            context.restore()
        }

        onCanvasSizeChanged: {
            if (canvas.available) {
                if (context !== null) {
                    // for scaling
                    scaleFactor = canvas.height / initHeight

                    // According to bug in QtQuick:
                    //   https://bugreports.qt-project.org/browse/QTBUG-39114
                    getContext('2d').reset()
                    requestPaint()
                }
            }
        }

        onWidthChanged: {
            canvas.requestPaint()
        }
        onHeightChanged: {
            canvas.requestPaint()
        }
    }

    states : [
        State {
            name : "left"
            StateChangeScript {
                script: setLeftEdge();
            }

        },
        State {
            name : "right"
            StateChangeScript {
                script: setRightEdge();
            }
        }

    ]
}
