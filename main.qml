import QtQuick 2.9
import QtQuick.Window 2.2
//import QtQuick 2.9 as HelloQuick
import HelloQuick 1.0 as HelloQuick

import QtQuick.Controls 2.3

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: "HelloQuick"

    property alias zoomValue: zoomSlider.value
    property alias rotationValue: rotationSlider.value

    HelloQuick.Rectangle {
        id: renderer
        anchors.fill: parent

        MouseArea {
            id: area
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 88
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.left: parent.left
            acceptedButtons: Qt.AllButtons
            onPositionChanged: renderer.mouseMoveEvent(mouse)
            onPressed: renderer.mousePressEvent(mouse)
            onReleased: renderer.mouseReleaseEvent(mouse)
            onWheel: renderer.wheelEvent(wheel)
        }

        Button {
            id: resetButton
            width: height
            text: "Reset"
            anchors.top: area.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            onClicked: renderer.reset()
        }

        Slider {
            id: zoomSlider
            anchors.top: area.bottom
            anchors.right: parent.right
            anchors.left: resetButton.right
            stepSize: 1
            from: -50
            to: 49
            value: 0
            onValueChanged: renderer.zoom(value)
        }

        Slider {
            id: rotationSlider
            anchors.bottom: parent.bottom
            anchors.top: zoomSlider.bottom
            anchors.right: parent.right
            anchors.left: resetButton.right
            stepSize: 1
            from: -50
            to: 49
            value: 0
            onValueChanged: renderer.rotate(value)
        }
    }
}
