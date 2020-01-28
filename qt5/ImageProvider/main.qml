import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick 2.12

Window {
    visible:true
    title:"Image Provider"
    width:480
    height: 640
    Flickable{
    anchors.fill: parent

    Column {
        Image{
            source:"image://myimageprovider/http://n.sinaimg.cn/mil/8_img/upload/a4a6e641/783/w950h633/20200115/705f-imztzhp0257486.png"
        }
        Image{
            source:"image://myimageprovider/http://n.sinaimg.cn/mil/8_img/upload/a4a6e641/664/w950h514/20200115/427e-imztzhp0257502.png"
        }
    }
    }
}