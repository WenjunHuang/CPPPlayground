import QtQuick 2.12

Image {
    source:"rocket.png"
    MouseArea {
        anchors.fill: parent
        drag.target: parent
        drag.axis: Drag.XAndYAxis
    }

}
