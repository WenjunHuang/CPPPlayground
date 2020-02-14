import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    visible:true
    title:"DelayButton"
    height:480
    width: 640

    ColumnLayout {
        DelayButton {
            id:next
            text:qsTr("Next")
            icon.source:"qrc:/audible.svg"
            onClicked: {

            }
        }
    }
}