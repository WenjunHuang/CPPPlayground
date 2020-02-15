import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    visible:true
    title:"ApplicationWindow"
    height:480
    width: 640

    header:TabBar {
        id:bar
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

    footer:TabBar{
        TabButton{
            text:qsTr("FooterHome")
            width:implicitWidth
            icon.source:"qrc:/bandcamp.svg"
        }
    }

    StackLayout {
        id:content
        anchors.fill: parent
        currentIndex: bar.currentIndex
        Pane {
            Text{
                text:"Hello"
            }
        }
        Pane {
            Text{
                text:"World"
            }
        }
        Pane{
            Text{
                text:"!!!"
            }
        }
    }

}