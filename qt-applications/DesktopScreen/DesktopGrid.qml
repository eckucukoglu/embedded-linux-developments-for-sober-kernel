import QtQuick 2.0
import QtQuick.Controls 1.3

Rectangle {
    id: rowItem
    width: 0
    height:0
    color: "transparent"
    property int page:0

    Rectangle{
        id: backgroundBlack
        visible: false
        anchors.fill: parent
        color: "#3b393e"
        opacity: 0.9
        z:1
    }

    BusyIndicator {
           id: busyIndication2
           visible:false
           anchors.centerIn: parent
           z:1
        }

    GridView{
       anchors.leftMargin: 36
       anchors.rightMargin: 8
       anchors.topMargin: 23
       anchors.bottomMargin: 17
       anchors.fill: parent
       cellHeight: 130
       cellWidth: 126
       interactive: false
       focus: true
       clip:true

       boundsBehavior: Flickable.StopAtBounds

       model: GridListModel{
           id: listModel
       }

      delegate: AppDelegate{

       }
   }
   function reload()
   {
       listModel.reload();
   }
}

