import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    visible:true
    title:"Button"
    height:480
    width: 640

    ColumnLayout {
        Button {
            id:next
            text:qsTr("Next")
            icon.source:"qrc:/audible.svg"
            onClicked: {

            }
        }
        ColumnLayout {
            ButtonGroup{
                id:childGroup
                exclusive:false
                checkState:parentBox.checkState
            }
            CheckBox {
                id: parentBox
                enabled:false
                text:qsTr("Parent")
                checkState: childGroup.checkState
            }

            CheckBox{
                checked:true
                text:qsTr("Child 1")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
            }
            CheckBox{
                text:qsTr("Child 2")
                leftPadding: indicator.width
                ButtonGroup.group: childGroup
            }
        }
    }
}