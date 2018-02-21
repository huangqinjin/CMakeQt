import QtQuick 2.9
import QtQuick.Window 2.2

Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("CMakeQt")

    Rectangle {
        id: renderer
        z: -1
        anchors.fill: parent
    }
}
