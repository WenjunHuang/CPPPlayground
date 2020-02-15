import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12

Window {
    visible:true
    title:"ComboBox"
    height:480
    width: 640

    QtObject {
        id:backend
        property int modifier
    }

    ColumnLayout {
        anchors.fill: parent
        ComboBox{
        Layout.alignment:Qt.AlignCenter
            model:["First","Second","Third"]
        }

        ComboBox{
            textRole:"text"
            valueRole:"value"
            onActivated: backend.modifier = currentValue
            model: [
                {value:Qt.NoModifier,text:qsTr("No modifier")},
                {value:Qt.ShiftModifier,text:qsTr("Shift")},
                {value:Qt.ControlModifier,text:qsTr("Control")}
            ]
        }

        ComboBox {
            id: control
            model: ["First","Second","Third"]
            delegate: ItemDelegate {
                width: control.width
                contentItem:Text{
                    text: modelData
                    color: "#21be2b"
                    font: control.font
                    elide: Text.ElideRight
                    verticalAlignment:Text.AlignVCenter
                }
                highlighted: control.highlightedIndex == index
            }
            indicator: Canvas {
                id: canvas
                x: control.width - width - control.rightPadding
                y: control.topPadding + (control.availableHeight - height) / 2
                width: 12
                height: 8
                contextType: "2d"

                Connections {
                    target:control
                    onPressedChanged: canvas.requestPaint()
                }

                onPaint:{
                    context.reset();
                    context.moveTo(0,0);
                    context.lineTo(width,0);
                    context.lineTo(width / 2,height);
                    context.closePath();
                    context.fillStyle = control.pressed ?"#17a81a":"#21be2b";
                    context.fill();
                }
            }
            contentItem: Text {
                leftPadding: 0
            }
        }


        Item{
            Layout.fillHeight:true
        }
    }

}