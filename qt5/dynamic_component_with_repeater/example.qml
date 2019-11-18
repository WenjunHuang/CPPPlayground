import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtQuick.Layouts 1.13
import QtQuick.Dialogs 1.2

import Foo 1.0

Window {
    visible:true
    width: 640
    height: 480

    color:Theme.primaryColor

    ColorDialog {
          id: colorDialog
          title: "Please choose a color"
          onAccepted: {
              console.log("You chose: " + colorDialog.color)
              Theme.primaryColor = colorDialog.color
          }
          onRejected: {
              console.log("Canceled")
          }
      }

    ColumnLayout{
        anchors.fill: parent

    Pane {
        Layout.fillWidth: true
        Layout.fillHeight: false
        spacing: Theme.spacing
        RowLayout {
            Text{
                text: colorMaker.colors.length + " colors"
            }

            Button{
                text:"Regenerate"
                onClicked: {
                    colorMaker.resetColors()
                }
            }

            Button {
                text:"Choose Primary"
                onClicked: {
                    colorDialog.open()
                }
            }

            Slider {
                id:slider
                from: 0
                to: 20
                value: Theme.spacing

                Binding {
                    target:Theme
                    property:"spacing"
                    value: slider.value
                }
            }

        }
    }

    ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true
        contentHeight: content.height
    ColumnLayout{
        id: content
        anchors.left: parent.left
        anchors.right:parent.right
        spacing: Theme.spacing
        Repeater {
            model: colorMaker.colors
            Rectangle {
                Layout.fillWidth: true
                         height: 40
                          border.width: 1
                          color: modelData
                          Text{
                              text:index
                              anchors.centerIn: parent
                          }
                      }
        }
    }
    }
}
}
