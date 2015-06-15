import QtQuick 2.4
import Qt3D 2.0
import Qt3D.Shapes 2.0
import Helpers 1.0

Viewport {
    id: view3dPerspective

    property alias meshColor: meshColorEffect.color
    property alias mesh0Source: mesh0.source
    property alias mesh1Source: mesh1.source
    property alias eye: cam.eye

    property vector3d rotations: Qt.vector3d(-60,-20,180)
    property int heightOfTheCamera: 0

    camera: Camera {
        id: cam
    }

    Q3DHelper {
        Component.onCompleted: {
            setNode(mesh0)

            cam.center = boundingBoxTrueCenter()

            if (mesh1Source != "") {
                cam.center.x -= boundingBoxTrueCenter().x
                itemRight.position.x += boundingBoxTrueCenter().x * 3
            }

            //after z rotation
            cam.center.x = cam.center.x * -1
            cam.center.y = cam.center.y * -1

            //after x,y rotation (not precision)
            cam.center.y = cam.center.y * 0.5
            cam.center.x = cam.center.x * 1.5

            heightOfTheCamera = boundingBoxFrontCenter().z + 100
            pinOut.restart()
        }
    }

    Mesh {
        id: mesh0
        dumpInfo: true
        options: "ForceSmooth"
        onLoaded: {
            itemLeft.effect = meshColorEffect
        }
    }

    Item3D {
        id: itemLeft
        mesh: mesh0

        position: Qt.vector3d(0, 0, 0)

        transform: [
            Rotation3D {
                angle: rotations.z
                axis: Qt.vector3d(0,0,1)
            },
            Rotation3D {
                angle: rotations.x
                axis: Qt.vector3d(1,0,0)
            },
            Rotation3D {
                angle: rotations.y
                axis: Qt.vector3d(0,1,0)

            }
        ]
    }

    Mesh {
        id: mesh1
        dumpInfo: true
        options: "ForceSmooth"
        onLoaded: {
            itemRight.effect = meshColorEffect
        }
    }

    Item3D {
        id: itemRight
        mesh: mesh1

        enabled: (mesh1Source != "")

        position: Qt.vector3d(0, 0, 0)

        transform: [
            Rotation3D {
                angle: rotations.z
                axis: Qt.vector3d(0,0,1)
            },
            Rotation3D {
                angle: rotations.x
                axis: Qt.vector3d(1,0,0)
            },
            Rotation3D {
                angle: rotations.y
                axis: Qt.vector3d(0,1,0)
            }
        ]
    }

    Effect {
        id: meshColorEffect
    }

    ParallelAnimation {
        id: pinOut

        NumberAnimation { target: view3dPerspective; properties: "eye.z"; to: heightOfTheCamera ; duration: 3000; easing.type: Easing.InQuad}
    }
}
