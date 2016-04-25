import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3
import CpuMemHandler 1.0
import "helperfunctions.js" as Logic

Item{
    //for the Processes
    property var statsString
    property string name
    property string mem
    property string pid
    property string memHumanReadable
    property var processEntry

    anchors.right: parent.right
    anchors.left: parent.left
    anchors.leftMargin: -50
    width: 380
    height: processListView.height

    Item{
        id: titleContainer
        width: parent.width
        height: 30

        SoberText{
            width: parent.width
            anchors.top: parent.top
            id: title
            font.pixelSize: 22
            height: 20
            color: "#04caad"
            text: "Processes"
        }

        CustomButton{
            id: refreshButton
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            text: "Refresh List"
            onClicked: {
                Logic.updateProcessesList()

            }
        }

    }

    ListView{
        id: processListView
        anchors.top: titleContainer.bottom
        anchors.topMargin: 20
        spacing: 10
        height: count * 55  //height of the view element is (number of items in it) * (one item delegate's height + spacing)

        delegate: ProcessDelegate{
                }
        model: processListModel

        //flickableDirection: Flickable.HorizontalFlick
    }

    ListModel{
        id: processListModel
    }


    Component.onCompleted: {
        Logic.updateProcessesList();

    }

    CpuMemHandler{
        id:cpuMemHandler
    }

//    Timer{
//        id:refresher
//        running: true
//        repeat: true
//        interval: 5000

//        onTriggered: {
//            cpuMemHandler.readAllStatFiles();
//            //Logic.updateProcessesList()   //LEAKY FUNCTION.....
//        }
//    }
}
