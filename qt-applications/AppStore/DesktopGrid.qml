import QtQuick 2.0

Rectangle {
    id: rowItem
    width: 0
    height:0
    color: "transparent"
    property int page:0
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
        listModel.fillListModel();
    }
}
