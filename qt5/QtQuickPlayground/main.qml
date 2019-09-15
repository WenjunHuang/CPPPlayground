import QtQuick 2.13
import QtQuick.Window 2.13

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Loader {
        id: _loader

        function reload(){
            source = ""
            $QmlEngine.clearCache()
            source = "Circle.qml"
        }
        anchors.centerIn: parent
        source:"Circle.qml"
    }

    MouseArea{
        anchors.fill:parent
        onClicked: {
            _loader.reload()
        }
    }
}
