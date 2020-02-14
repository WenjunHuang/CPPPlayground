import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

Window {
    visible:true
    title:"Switch"
    height:480
    width: 640

    ColumnLayout {
        id: columns
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Switch {
            text:qsTr("Wi-Fi")

            contentItem: Text{
                text: parent.text
                color: parent.checked? Material.primary:Material.accent
                leftPadding: parent.indicator.width + parent.spacing
                verticalAlignment:Text.AlignVCenter
            }
        }
        Switch {
            text:qsTr("Bluetooth")
        }
    }

    ListView{
        id:listView
        anchors.top: columns.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model:Qt.fontFamilies()

        delegate:SwitchDelegate{
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