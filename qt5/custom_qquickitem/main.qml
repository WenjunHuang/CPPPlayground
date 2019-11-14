import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQml 2.2
import ClockCircle 1.0

Window{
visible:true
width:400
height:400

ClockCircle{
id:clockCircle
anchors.top:parent.top
anchors.topMargin: 50
anchors.horizontalCenter:parent.horizontalCenter
width:200
height:200
name:"clock"
backgroundColor:"whiteSmoke"
borderActiveColor:"LightSlateGray"
borderNonActiveColor:"LightSteelBlue"

Text{
id:textTimer
anchors.centerIn: parent
font.bold:true
font.pixelSize: 24
}

onCircleTimeChanged:{
textTimer.text = Qt.formatTime(circleTime,"mm:ss.zzz")
}

MouseArea {
anchors.fill:parent
onClicked:{
if (clockCircle.isRunning()){
    clockCircle.stop();
} else{
    clockCircle.start();
}
}

}
}

Button {
id:start
text:"Start"
onClicked: clockCircle.start();
anchors {
    left: parent.left
    leftMargin: 20
    bottom: parent.bottom
    bottomMargin: 20
}
}

Button {
    id:stop
    text:"Stop"
    onClicked:clockCircle.stop()
    anchors {
        horizontalCenter: parent.horizontalCenter
        bottom: parent.bottom
        bottomMargin: 20
    }
}

Button {
    id: clear
    text:"Clear"
    onClicked: clockCircle.clear()
    anchors {
    right:parent.right
    rightMargin: 20
    bottom:parent.bottom
    bottomMargin:20
    }
}
}