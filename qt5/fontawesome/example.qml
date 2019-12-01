import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Window 2.13
import QtQuick.Layouts 1.13
import Foo 1.0

Window {
    width:480
    height: 640
    visible:true

RowLayout{
anchors.centerIn:parent
Text {
font.family: FAFonts.solidIcons
font.weight: Font.Bold
text:FAIcons.faAcorn
}
Text {
font.family: FAFonts.regularIcons
font.weight:Font.Normal
font.pixelSize: 72
text:FAIcons.faAcorn
color:"orange"
}
Text {
font.family: FAFonts.lightIcons
font.weight: Font.Light
font.pixelSize: 72
text:FAIcons.faAcorn
color:"blue"
rotation: -45
}
}
}
