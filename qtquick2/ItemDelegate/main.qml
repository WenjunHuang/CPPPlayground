import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    visible:true
    title:"DelayButton"
    height:480
    width: 640

    ListView{
        id:listView
        anchors.fill: parent
        model:Qt.fontFamilies()

        delegate:ItemDelegate{
            text:modelData
            width: parent.width
            icon.source:"qrc:/docker.svg"
            onClicked:{console.log("clicked",modelData);
            listView.currentIndex = index;

            }
            highlighted: ListView.isCurrentItem
        }
        ScrollIndicator.vertical: ScrollIndicator{}
    }
}