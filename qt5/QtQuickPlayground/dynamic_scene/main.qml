import QtQuick 2.12
import QtQuick.XmlListModel 2.12
import "create-object.js" as CreateObject

Item {
    id: root
    ListModel {
        id: objectsModel
    }

    function addUfo() {
        const builder = new CreateObject.ObjectBuilder("ufo.qml",root,itemAdded)
        builder.create()
    }

    function addRocket() {
        const builder = new CreateObject.ObjectBuilder("rocket.qml",root,itemAdded)
        builder.create()
    }

    function itemAdded(obj,source) {
        objectsModel.append({ "obj":obj, "source":source })
    }

    function clearItems(){
        while (objectsModel.count > 0) {
            objectsModel.get(0).obj.destroy()
            objectsModel.remove(0)
        }
    }

    function serialize(){
        const res = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<scene>\n"
        for (var ii = 0; ii < objectsModel.count; ++ii) {
            const i = objectsModel.get(ii)
            res += `<item><source>${i.source}</source><x>${i.obj.x}</x><y>${i.obj.y}</y></item>`
        }
        res += "</scene>"
        return res
    }

    function deserialize() {
        dsIndex = 0;
        const builder = new CreateObject.ObjectBuilder(
                          xmlModel.get(dslIndex).source,
                          root,
                          dsItemAdded)
        builder.create()
    }

    function dsItemAdded(obj,source) {
        itemAdded(obj,source)
        obj.x = xmlModel.get(dsIndex).x
        obj.y = xmlModel.get(dsIndex).y

        dsIndex++

        if (dsIndex < xmlModel.count) {
            const builder = new CreateObject.ObjectBuilder(
                              xmlModel.get(dsIndex).sorce,
                              root,dsItemAdded)
        }
    }
    property int dsIndex


    XmlListModel {
        id: xmlModel
        query:"/scene/item"
        XmlRole{name:"source";query:"source/string()"}
        XmlRole{name:"x";query:"x/string()"}
        XmlRole{name:"y";query:"y/string()"}
    }

    width: 1024
    height: 600

    Column {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 10
        spacing: 10
        width: 100

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "ufo.png"

            MouseArea {
                anchors.fill: parent
                onClicked: addUfo()
            }
        }

        Image {
            anchors.horizontalCenter: parent.horizontalCenter
            source:"rocket.png"
            MouseArea {
                anchors.fill: parent
                onClicked: addRocket()
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 40
            color: "#53d769"

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    xmlModel.xml = serialize()
                    clearItems()
                }
            }
        }

        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter
            width: 100
            height: 40
            color:"#fed958"
            MouseArea {
                anchors.fill: parent
                onClicked: deserialize()
            }
        }
    }

}
