// View3dPosting.qml

import QtQuick 2.0
import QtQuick.Layouts 1.1
import Qt3D 2.0
import Qt3D.Shapes 2.0

import Helpers 1.0

Rectangle {
    id: view3dPosting
    color: "transparent"

    property alias meshColor: meshColorEffect.color
    property alias meshSource: commonMesh.source
    property color commonFillColor: "transparent"
    property alias uView: upperView
    property alias bView: bottomView
    property bool commonNavigation: true

    Mesh {
        id: commonMesh
        dumpInfo: true
        options: "ForceSmooth"
        onLoaded: {
            uItem.effect = meshColorEffect
            bItem.effect = meshColorEffect

            q3dhelper.setNode(commonMesh)
        }
    }

    Effect { id: meshColorEffect }

    Q3DHelper { id: q3dhelper }

    ColumnLayout {
        id: column
        anchors {
            fill: parent
            margins: 15
        }

        Rectangle {
            id: upperRect
            clip: true

            Layout.fillHeight: true
            Layout.fillWidth: true

            border {
                color: "black"
                width: 1
            }
            radius: 10

            color: "transparent"

            Viewport {
                id: upperView
                anchors.fill: upperRect
                anchors.margins: 20
                navigation: commonNavigation
                fillColor: commonFillColor

                property alias cameraNP: uViewCam.nearPlane
                property int distance: 10
                property int k: 65 // scale factor
                property alias cam: uViewCam

                camera: Camera {
                    id: uViewCam
                    projectionType: Camera.Orthographic
                    eye: Qt.vector3d(0, 0, bottomView.distance)
                }

                Item3D {
                    id: uItem
                    mesh: commonMesh
                    position: Qt.vector3d(1.5, -0.7, 0)
                    scale: 1 / upperView.k
                    transform : [
                        Rotation3D {
                            angle: -90
                            axis: Qt.vector3d(1, 0, 0)
                        },
                        Rotation3D {
                            angle: 90
                            axis: Qt.vector3d(0, 1, 0)
                        }
                    ]
                }

                StickyDegreeLabel {
                    id: upperViewDegree
                    degree: 0
                }
            }
        }

        Text {
            Layout.fillWidth: true
            text: "lateral"
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: bottomRect
            clip: true

            Layout.fillHeight: true
            Layout.fillWidth: true

            border {
                color: "black"
                width: 1
            }
            radius: 10
            color: "transparent"

            Viewport {
                id: bottomView
                anchors.fill: bottomRect
                anchors.margins: 20
                navigation: commonNavigation
                fillColor: commonFillColor

                property alias cameraNP: bViewCam.nearPlane
                property int distance: 10
                property int k: 65 // scale factor
                property alias cam: bViewCam

                camera: Camera {
                    id: bViewCam
                    projectionType: Camera.Orthographic
                    eye: Qt.vector3d(0, 0, bottomView.distance)
                }

                Item3D {
                    id: bItem
                    mesh: commonMesh
                    position: Qt.vector3d(0.8, -0.2, 0)
                    scale: 1 / bottomView.k

                    transform : [
                        Rotation3D {
                            angle: -90
                            axis: Qt.vector3d(1, 0, 0)
                        },
                        Rotation3D {
                            angle: 180
                            axis: Qt.vector3d(0, 1, 0)
                        }
                    ]
                }

                StickyDegreeLabel {
                    id: bottomViewDegree
                    degree: 0
                }
            }
        }

        Text {
            Layout.fillWidth: true
            text: "anterior/posterior"
            horizontalAlignment: Text.AlignHCenter
        }
    }
}
