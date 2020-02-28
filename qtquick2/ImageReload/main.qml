import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.13

Window {
    visible:true
    title:"DelayButton"
    height:480
    width: 640
Item {
anchors.fill: parent
    Image {
    id: target
        anchors.fill: parent
        source: "image://imageprovider/picture.jpg"
    }
    // 加入一点颜色混合，使得"播放数"文字能比较突出
    LinearGradient {
        id: gradient
        anchors.fill: target
        start: Qt.point(0, 0)
        end: Qt.point(0, 300)
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#000000"
            }
            GradientStop {
                position: 0.3
                color: "white"
            }
        }
        visible: false
    }
    Blend {
        anchors.fill: target
        source: target
        foregroundSource: gradient
        mode: "darken"
    }
}


}