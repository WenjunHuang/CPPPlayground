import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.13
import ViewModel 1.0

Window {
width: 480
height: 640
visible:true

SetuViewModel {
    id:viewModel
//    onStimulateChanged: {
//    console.log(stimulate.type)
//    console.log(Helper.SeTu)
//    }
    Component.onCompleted:{
        console.log(allSetu)
//        for (let setu of allSetu) {
//            console.log(setu)
//        }
    }
}

Component {
    id:showSetu
    Image{
        source:loader.stimulate.setuUrl
    }
}

Component {
    id:showText
    Text{
        text:loader.stimulate.text
    }
}

ColumnLayout {
    anchors.fill: parent
    Button {
        Layout.fillWidth: true
        Layout.fillHeight: false
        Layout.alignment: Qt.AlignTop|Qt.AlignHCenter
        text:"Switch me"
        onClicked:{
            viewModel.randomStimulate();
        }
    }

    Item{
        id:content
        Layout.fillWidth:true
        Layout.fillHeight:true

        Loader{
        id:loader
        anchors.fill: parent
        property var stimulate:viewModel.stimulate
        }

        state: viewModel.stimulate.type == Helper.SeTu ? "setu":"text"

        states: [
            State{
                name:"setu"
                PropertyChanges{target:loader;sourceComponent:showSetu;explicit:true}
            },
            State{
                name:"text"
                PropertyChanges{target:loader;sourceComponent:showText;explicit:true}
            }
        ]
    }
}

}

