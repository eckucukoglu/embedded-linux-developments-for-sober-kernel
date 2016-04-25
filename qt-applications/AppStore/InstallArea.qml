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
  property string app_name_str
  property string border_color
  property string dev_name
  property string icon_path
  property string color
  property int app_id
  property int already_installed:0
  property int retValue:0
  property int index
  property string date
  property string version

  Rectangle
  {
       anchors.fill: parent
       border.width: 0.5
       border.color: border_color
       color: "#3b393e"
       id: installWin
       radius: parent.radius
       width: parent.width
       Rectangle{
           id: appIcon
           x:20
           y:20
           width: 100
           color: 'transparent'
           height: 100
           Rectangle{
               id: app_overlay
               width: 100
               height: 100
               radius: width * 0.5
               border.width: 2
               border.color: border_color
               color: "transparent"
               Rectangle{
                   id: app_gradient
                   width: parent.width
                   height: parent.height
                   radius: parent.radius
                   color: "transparent"
                   opacity: 0.9

                   RadialGradient {
                       anchors.fill: parent
                       horizontalRadius: 84
                       verticalRadius: 84
                       gradient: Gradient {
                           GradientStop { position: 0.0; color: border_color}
                           GradientStop { position: 0.5; color: "transparent" }
                       }
                   }
               }
              Image {
                   id: app_icon
                   source: icon_path
                   anchors.centerIn: parent
                   anchors.horizontalCenter: parent.horizontalCenter
               }
           }
       }
       Text{
           id: appName
           color: "white"
           anchors.horizontalCenter: parent.horizontalCenter
           anchors.top: parent.top
           anchors.topMargin: 20
           text: app_name_str
           font.bold: true
       }
       Text{
           id: devName
           color: "white"
           anchors.topMargin: 20
           anchors.top: appName.bottom
           anchors.left: appIcon.right
           anchors.leftMargin: 30
           text: "Developer: " + dev_name
       }
       Text{
           id: versionInfo
           color: "white"
           anchors.topMargin: 20
           anchors.top: appName.bottom
           anchors.right: installWin.right
           anchors.rightMargin: 30
           text: "Version: " + version
       }
       Text{
           id: dateInfo
           color: "white"
           anchors.topMargin: 10
           anchors.top: devName.bottom
           anchors.left: appIcon.right
           anchors.leftMargin: 30
           text: "Created Date: " + date
       }

       Rectangle{
           id: progressBarField
           visible: false
           anchors.top: appIcon.bottom
           anchors.bottom: installBtn.top
           anchors.left: installWin.left
           anchors.topMargin: 30
           anchors.bottomMargin: 20
           anchors.leftMargin: 40
           anchors.rightMargin: 40
           width: 420
           height: 35
           Text{
               text: appName.text + " app is downloading.."
               anchors.bottom: progressBar.top
               anchors.bottomMargin: 2
               font.pixelSize: 12
           }
           ProgressBar {
              id:progressBar
              anchors.fill: parent
              indeterminate: true
              style: ProgressBarStyle {
                  background: Rectangle {
                      radius: 2
                      color: "lightgray"
                      border.color: "gray"
                      border.width: 1
                      implicitWidth: 200
                      implicitHeight: 24
                  }
              }
           }
       }

       CustomButton{
           id: installBtn
           text: "DOWNLOAD"
           visible: true
           anchors.left: parent.left
           anchors.bottom: parent.bottom
           anchors.leftMargin: installWin.width/2 - width
           anchors.bottomMargin: 30
           width: 100
           onClicked: {
               progressBarField.visible=true
               errorTxt.visible=false
               alreadyInsText.visible=false
               nowInstalledTxt.visible=false
               console.log("downloading: "+ app_id)
               AppsModel.download(app_id)
               if(AppsModel.check_error() === 0) //download is successful
               {
                   progressBarField.visible=false
                   nowInstalledTxt.visible=true
                   installBtn.visible=false
                   cancelBtn.visible=false
                   okBtn.visible=true
               }
               if(AppsModel.check_error() === 1) //error on download
               {
                   progressBarField.visible=false
                   errorTxt.visible=true
               }
            }
       }

       CustomButton{
           id: cancelBtn
           text: "CANCEL"
           visible:true
           width: 100
           anchors.left: installBtn.right
           anchors.leftMargin: 30
           anchors.bottom: parent.bottom
           anchors.bottomMargin: 30
           onClicked: {
               installArea.visible=false
               content.enabled=true
               swipeArea.enabled=true

           }
       }

       Text{
           id: alreadyInsText
           anchors.bottom: okBtn.top
           anchors.bottomMargin: 20
           color: "white"
           font.bold: true
           visible: false
           text: appName.text + " app is already installed."
           anchors.horizontalCenter: parent.horizontalCenter
       }

       Text{
           id: nowInstalledTxt
           anchors.bottom: okBtn.top
           anchors.bottomMargin: 20
           font.bold: true
           color: "white"
           visible: false
           text: appName.text + " app has successfully installed."
           anchors.horizontalCenter: parent.horizontalCenter
       }
       Text{
           id: errorTxt
           anchors.bottom: okBtn.top
           anchors.bottomMargin: 20
           font.bold: true
           visible: false
           color: "red"
           text: "Download Failed!"
           anchors.horizontalCenter: parent.horizontalCenter
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
           onClicked: {
               installArea.visible=false
               content.enabled=true
               swipeArea.enabled=true
          }
       }
   }
  onVisibleChanged:{
       backgroundBlack.visible = visible
       already_installed = AppsModel.check_if_installed(app_id)
       console.log("app id is : "+ app_id)
       console.log("installed: "+ already_installed)
       if(already_installed === 0) //app is not already installed
       {
           okBtn.visible=false
           cancelBtn.visible=true
           installBtn.visible=true
           alreadyInsText.visible=false
           errorTxt.visible=false
           nowInstalledTxt.visible=false
       }
       else if(already_installed === 1) //app is already installed
       {
           okBtn.visible=true
           cancelBtn.visible=false
           installBtn.visible=false
           alreadyInsText.visible=true
           nowInstalledTxt.visible=false
           errorTxt.visible=false
       }
  }
  Component.onCompleted: {
      already_installed =AppsModel.check_if_installed(app_id)
      console.log("app id is : "+ app_id)
      console.log("installed: "+ already_installed)
      if(already_installed === 0) //app is not installed
      {
          okBtn.visible=false
          cancelBtn.visible=true
          installBtn.visible=true
          alreadyInsText.visible=false
          errorTxt.visible=false
          nowInstalledTxt.visible=false
      }
      else if(already_installed === 1) //app is already installed
      {
          okBtn.visible=true
          cancelBtn.visible=false
          installBtn.visible=false
          alreadyInsText.visible=true
          nowInstalledTxt.visible=false
          errorTxt.visible=false
      }
  }
}

