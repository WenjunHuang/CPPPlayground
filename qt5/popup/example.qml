import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtQuick.Layouts 1.13
import QtQuick.Controls.Material 2.13
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.13

ApplicationWindow {
    visible:true
    width: 640
    height: 480
    Material.accent: Material.Purple

    RoundButton{
    id:button
    radius: height/2
    leftPadding: radius
    rightPadding: radius
        text:"全部歌单"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 10
        anchors.topMargin: 10
        onClicked: popup.open()

        Behavior on width {
            NumberAnimation {duration:2000}
        }

    Popup {
    id:popup
        x: 0
        y: parent.height
        ColumnLayout{
        anchors.fill:parent
        CheckBox{
            text:"E-mail"
        }
        CheckBox{
            text:"Calendar"
        }
        CheckBox{
            text:"Contacts"
        }
        }
    }
    }

    Button {
        id:change
        anchors.centerIn:parent
        text:"随机修改宽度"
        onClicked:{
            const times = Math.floor(Math.random() * 10)
            let text = ""
            for (let t = 0;t < times;t++)
                text += "全部歌单"
            button.text = text
        }
    }

}
