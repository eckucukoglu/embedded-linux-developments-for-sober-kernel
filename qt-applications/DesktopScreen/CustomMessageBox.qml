import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.4

Rectangle{
  width: 500
  height: 250
  visible: false
  enabled: true
  radius: 10

  Rectangle
  {
       anchors.fill: parent
       border.width: 0.5
       border.color: border_color
       color: "#3b393e"
       id: messageBoxArea
       radius: parent.radius
       width: parent.width

       Text{
           id: infoText
           color: "white"
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.top: parent.top
           anchors.topMargin: 20
           text: ""
           font.bold: true
       }

       CustomButton{
           id: acceptBtn
           text: "Accept"
           visible: true
           anchors.left: parent.left
           anchors.bottom: parent.bottom
           anchors.leftMargin: installWin.width/2 - width
           anchors.bottomMargin: 30
           width: 100

           onClicked: {
               messageBoxArea.visible = false
            }
       }

       CustomButton{
           id: cancelBtn
           text: "CANCEL"
           visible:false
           width: 100
           anchors.left: installBtn.right
           anchors.leftMargin: 30
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 30
           onClicked: {
              messageBoxArea.visible = false
           }
       }

       CustomButton{
           id: okBtn
           text: "OK"
           visible: false
           anchors.left: installBtn.right
           anchors.leftMargin: 30
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 30
           width: 100

       }
   }
  onVisibleChanged:{
       backgroundBlack.visible = visible
  }
  Component.onCompleted: {

  }

  function setText(text){
        infoText.text = text
  }
  function setAcceptCancelMode(){
       acceptBtn.visible = true
       cancelBtn.visible = true
       okBtn.visible = false
  }
  function setOkMode(){
      acceptBtn.visible = false
      cancelBtn.visible = false
      okBtn.visible = true
  }


}

