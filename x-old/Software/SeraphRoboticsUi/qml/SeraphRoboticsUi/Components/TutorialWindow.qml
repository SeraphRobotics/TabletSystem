pragma Singleton
import QtQuick 2.4
import QtWebKit 3.0

Item {
    id : background

    enabled: opacity === 1 ? true : false

    anchors.fill: parent

    /// @note always on top
    z: 9999
    opacity: 0

    function showPopup(parentElement)
    {
        background.opacity = 1
    }

    MouseArea {
        anchors.fill: parent
    }

    GroupBoxTemplate {
        id: tutorialWindow

        signal play
        property alias ulr: webView.url

        anchors.centerIn : parent
        width : 520
        height: 360

        enabled: opacity === 1 ? true : false
        title: "Tutorial"
        opacity: background.opacity

        z : 99

        Text {
            anchors {
                right: tutorialWindow.header.right
                rightMargin: parent.width * 0.03
                verticalCenter: tutorialWindow.header.verticalCenter
                verticalCenterOffset: -5
            }
            text: "X"
            color: "white"
            font.pixelSize: tutorialWindow.headerTextFontPixelSize + 2
            font.bold: true

            MouseArea {
                anchors.fill: parent

                cursorShape : Qt.PointingHandCursor

                onClicked: {
                    background.opacity = 0
                    webView.stop()
                }
            }
        }

        WebView {
            id : webView

            anchors {
                verticalCenter: parent.verticalCenter
                verticalCenterOffset: tutorialWindow.headerTextFontPixelSize
                horizontalCenter: parent.horizontalCenter
            }

            width: parent.width * 0.85
            height: parent.height * 0.75

            url : ""

            Component.onCompleted: {
                webView.stop()
            }
        }

        onPlay: {
            webView.reload()
        }

        Behavior on opacity { NumberAnimation {} }

    }
}
