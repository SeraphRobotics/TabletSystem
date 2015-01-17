import QtQuick 2.4

Item {
    id : mainElement

    width : 20
    height : 20

    property real centerX : (m_Controler.x + m_Controler.width/2)
    property real centerY : (m_Controler.y + m_Controler.height/2)

    signal customPressed()

    // Value used for storing a pad that is currently being rotated.
    property Item m_Controler : null
    // Value used for storing an image border that is currently being rotated.
    property Item imageBorder : null

    parent : m_Controler.parent

    // Value used to set basic rotation.
    property alias oldRotation: mouse_area.oldRotation
    property alias mouse_area: mouse_area

    z : m_Controler.z

    clip : true

    Image {
        id : rotateElement

        source : "qrc:/QmlResources/rotate.svg"
        smooth : true

        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        scale : 0.7
    }
    MouseArea {
        id : mouse_area

        objectName : "draggableElement"

        property int oldRotation: 0

        anchors
        {
            centerIn: rotateElement
        }

        width : 20
        height : 20
        clip : true

        property real centerXPointDiffrent :0
        cursorShape: Qt.PointingHandCursor

        onPressed:
        {
            oldRotation = m_Controler.rotation
            centerXPointDiffrent = mouse_area.width/2 - mouseX
        }

        onPositionChanged:
        {
            var point =  mapToItem (mainElement.parent, mouse.x, mouse.y);
            var diffX = (point.x - mainElement.centerX)+centerXPointDiffrent;
            var diffY =  (mainElement.centerY- point.y);
            var rad = Math.atan (diffY / diffX);
            var deg = (rad * 180 / Math.PI);

            if (diffX > 0 && diffY > 0)
            {
                m_Controler.rotation = 90 - Math.abs (deg) + oldRotation;
            }
            else if (diffX > 0 && diffY < 0)
            {
                m_Controler.rotation = 90 + Math.abs (deg) + oldRotation;
            }
            else if (diffX < 0 && diffY > 0)
            {
                m_Controler.rotation = 270 + Math.abs (deg) + oldRotation;
            }
            else if (diffX < 0 && diffY < 0)
            {
                m_Controler.rotation = 270 - Math.abs (deg )+ oldRotation;
            }

            imageBorder.boxSizeRotate()
            imageBorder.recalculate()
        }

    }
}
