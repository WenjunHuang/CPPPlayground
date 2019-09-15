import QtQuick 2.12

Image {
    source:"ufo.png"

    MouseArea {
        anchors.fill: parent

        drag.target: parent
        drag.axis: Drag.XAndYAxis
    }

}
