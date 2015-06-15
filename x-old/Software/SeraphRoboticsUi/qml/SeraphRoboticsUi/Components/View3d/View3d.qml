// View3d.qml

import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0
import Helpers 1.0


// 3dView Item - Viewer for stl files.
// The most important limation here is that width/height ratio should always be 0.533,
// otherwise there are some weird problems with scaling

Viewport {
    id: view3d

    property alias meshColor: meshColorEffect.color
    property alias meshSource:  mesh.source
    property alias cameraNP: cam.nearPlane
    property int distance: 6
    property int k: 51 // scale factor
    property int timeParametr : 61

    navigation : false

    camera: Camera {
        id: cam

        projectionType: Camera.Orthographic
        eye: Qt.vector3d(0, 0, distance)
    }

    Q3DHelper {
        id: q3dhelper
    }

    Mesh {
        id: mesh
        dumpInfo: true
        options: "ForceSmooth"
        onLoaded: {
            item.effect = meshColorEffect

            q3dhelper.setNode(mesh)

            cam.center = q3dhelper.boundingBoxCenter().times(1/timeParametr)
            cam.eye = cam.center
            cam.eye.z = distance
        }
    }

    Item3D {
        id: item

        mesh: mesh
        position: Qt.vector3d(0, 0, 0)
        scale: 1 / k
    }

    Effect {
        id: meshColorEffect
    }

    onTimeParametrChanged: {
        cam.center = q3dhelper.boundingBoxCenter().times(1/timeParametr)
        cam.eye = cam.center
        cam.eye.z = distance
    }
}
