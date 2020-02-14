import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    visible:true
    title:"Button"
    height:480
    width: 640

    TabBar {
        anchors.top: parent.top
        TabButton{
            text:qsTr("Home")
            width: implicitWidth
            icon.source: "qrc:/audible.svg"

        }
        TabButton{
            width: implicitWidth
            text:qsTr("Discover")
        }
        TabButton {
            width: implicitWidth
            text:qsTr("Activity")
        }
    }

}