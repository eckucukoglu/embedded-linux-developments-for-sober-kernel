import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.2

ApplicationWindow {
    title: qsTr("Memory Tester")
    width: 800
    height: 480
    visible: true
    property int size: 100

  Rectangle{
      anchors.fill: parent
      color: "#3b393e"
  }

  Text{
      id: titleTxt
      font.family: "Helvetica"
      text: "Memory Tester"
      color: "white"
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      font.pixelSize: 24
      anchors.topMargin: 60
  }


    Rectangle{
       id: infoArea
       color: "#666569"
       width: 300
       height: 40
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.bottom: btn.top
       anchors.bottomMargin: 40
       radius: 4

       Text{
           id: infoTxt
           color: "white"
           font.family: "Helvetica"
           text: ""
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.verticalCenter: parent.verticalCenter
       }
   }

   CustomButton{
       id: btn
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.verticalCenter: parent.verticalCenter
       width: 300
       height: 40
       text: "Allocate size " + size
       onClicked: {
           var ret = helper.allocate()
           if(ret)
           {
              infoTxt.text = (size*1024) + " bytes allocated"
              size = helper.get_size()
           }
           else
           {
              infoTxt.text = "error"
           }
       }
   }

   CustomButton{
       id: freebtn
       anchors.horizontalCenter: parent.horizontalCenter
       anchors.top: btn.bottom
       anchors.topMargin: 40
       width: 300
       height: 40
       text: "Free"
       onClicked: {
           var ret = helper.free_buffer()
           if(ret)
           {
               infoTxt.text = "Memory freed."
               size=helper.get_size()

           }
           else
           {
               infoTxt.text = ""
           }
      }
   }

   Item{
       width: 800
       height:36
       id: navigationBar
       //color: "transparent"
       anchors.bottom: parent.bottom
    //   anchors.top: row.bottom
       Rectangle{
           anchors.fill: parent
           Image {
               id: home_icon
               source: "home_icon.png"
               x: (parent.width/2)-16
               z:1
               anchors.horizontalCenter: parent.horizontalCenter
               scale: 1.2
           }
           Button{
               x: (parent.width/2)-18
               width: 36
               height: 36
               opacity: 0
               z: 1
               anchors.horizontalCenter: parent.horizontalCenter
               onClicked: {
                   Qt.quit()
               }
           }

           Rectangle{
               color: "#666569"
               anchors.fill: parent
           }
       }

   }

   CustomMessageBox{
       id: customMsgBox
       anchors.verticalCenter: parent.verticalCenter
       anchors.horizontalCenter: parent.horizontalCenter
   }
}
