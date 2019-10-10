import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
//import Exported 1.0
import Foo 1.0

Window {
    width: 640
    height: 280
    title: "Python Async"
    visible: true

    Foo{
    name: 'abc'
    }

    Item {
        id: root
        anchors.fill: parent
        anchors.margins: 6

        TextField {
            id: inputUrl
            text: "https://jsonplaceholder.typicode.com/todos/1"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            selectByMouse: true
        }

        ScrollView {
            anchors.top: inputUrl.bottom
            anchors.topMargin: 6
            anchors.bottom: btnGet.top
            anchors.left: parent.left
            anchors.right: parent.right
            TextArea {
                id: taResponse
                placeholderText: qsTr("Enter description")
            }
        }

        Button {
            id: btnGet
            text: "Get"
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                var obj = exportedFactory.create(inputUrl.text)
                if (obj) {
                    root.state = "loading"
                    var result = http.fetch(obj, function (result) {
                    console.log(result.result)

                        taResponse.text = result.result
                        root.state = "normal"
                    })
                }
            }
        }



        Rectangle {
            id: busy
            color: hexToRgbA('#ea4aaa', 0.2)
            opacity: 0.0
            enabled: opacity == 1.0
            anchors.fill: parent
            BusyIndicator {
                anchors.centerIn: parent
            }

            Behavior on opacity {
                NumberAnimation {
                    duration: 1000
                }
            }

            MouseArea {
                anchors.fill: parent
            }

            function hexToRgbA(hex,alpha) {
                var c
                if (/^#([A-Fa-f0-9]{3}){1,2}$/.test(hex)) {
                    c = hex.substring(1).split('')
                    if (c.length === 3) {
                        c = [c[0], c[0], c[1], c[1], c[2], c[2]]
                    }
                    c = '0x' + c.join('')
                    return Qt.rgba(((c >> 16) & 255) / 255, ((c >> 8) & 255) / 255,
                                   (c & 255) / 255, alpha)
                }
                throw new Error('Bad Hex')
            }
        }

        state: "normal"
        states: [
            State {
                name: "loading"
                PropertyChanges {
                    target: btnGet
                    enabled: false
                }
                PropertyChanges {
                    target: busy
                    opacity: 1.0
                }
            },
            State {
                name: "normal"
            }
        ]
    }
}
