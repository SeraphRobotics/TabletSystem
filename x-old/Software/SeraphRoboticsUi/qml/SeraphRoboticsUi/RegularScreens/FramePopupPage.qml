pragma Singleton

import QtQuick 2.4
import QtWebKit 3.0

import ".."
import "../Components"

// Frame popup window. Page 13 from pdf.
PageTemplate {
    id : tutorialsPage

    logo.enabled: false

    opacity: 0

    // Value used to make sure that popup is always on top.
    z : 9999

    enabled: opacity === 1 ? true : false

    MouseArea {
        anchors.fill: parent
    }

    function showPopup()
    {
        tutorialsPage.opacity = 1
    }

    ImageBasedButton {
        id : closeButton

        width : sourceSize.width
        height : sourceSize.height

        smooth: true
        anchors {
            right : parent.right
            rightMargin : 20
            top : parent.top
            topMargin: 20
        }

        onCustomClicked: {
            tutorialsPage.opacity = 0
        }
    }

    Rectangle {
        id : frameArea

        radius : 10

        anchors {
            top : parent.top
            topMargin :100
            bottom : parent.bottom
            bottomMargin: 100
            left : parent.left
            leftMargin: 100
            right : parent.right
            rightMargin: 100
        }

        border {
            width : 1
            color: "black"
        }

        WebView {
            id : webView

            anchors {
                top : parent.top
                topMargin :10
                bottom : parent.bottom
                bottomMargin: 10
                left : parent.left
                leftMargin: 10
                right : parent.right
                rightMargin: 10
            }

            url : qmlCppWrapper.iFrameUrl
        }
    }
}
