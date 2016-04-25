import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.0

Component {
    id: appDelegate
    Rectangle {
        id: appDelegateRect
        width: 90
        height: 90
        color: "transparent"      
        x:0
        Column {
            width: parent.width
            height: parent.height
            spacing: 6

            Rectangle{
                id: overlay
                width: 84
                height: 84
                radius: width * 0.5
                border.width: 2
                border.color: borderColor
                color: "transparent"

                Rectangle{
                    id: _gradient
                    width: parent.width
                    height: parent.height
                    radius: parent.radius
                    color: "transparent"
                    opacity: 0.45

                    RadialGradient {
                        anchors.fill: parent
                        horizontalRadius: 84
                        verticalRadius: 84
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: borderColor }
                            GradientStop { position: 0.5; color: "transparent" }
                        }
                    }
                }

                Image {
                    id: appIcon
                    source: portrait
                    anchors.centerIn: parent
                    anchors.horizontalCenter: parent.horizontalCenter
                    scale: 0.7
                }

                Rectangle{
                    id: deleteIcon
                    visible:root.isDeleteMode
                    width: 20
                    height: 20
                    anchors.right: parent.right
                    anchors.top: parent.top
                    radius: width * 0.5
                    border.width: 2
                    border.color: "red"
                    color: "transparent"
                    Rectangle{
                        anchors.fill: parent
                        color: "red"
                        opacity: 0.2
                    }

                    Text{
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        text: "X"
                        font.bold: true
                        font.pixelSize: 16
                        color: "red"
                    }
                    onVisibleChanged: {
                        if(app_id === 0)
                        {
                           visible=false
                        }
                    }
                }
            }



            Text {
                id: appName
                text: name
                width: parent.width + 6
                elide: Text.ElideRight

                anchors.horizontalCenter: parent.horizontalCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 12
                font.family: "Helvetica"
                color: "white"
            }
        }
      MessageDialog {
                id: msg
                title: "Title"
                text: "index:" + appDelegate.GridView.view.currentIndex
                onAccepted: visible = false
      }
      MessageDialog {
                id: error_msg
                title: "Error"
                visible:false
                icon: StandardIcon.Warning
                text: ""
                onAccepted: visible = false
      }
      MessageDialog {
                id: deleteMsg
                title: "Warning"
                visible:false
                icon: StandardIcon.Warning
                text: "Are you sure to delete " + name + " ?"
                standardButtons: StandardButton.Yes | StandardButton.No
                onYes: {
                    deleteMsg.visible=false
                    //TODO: refresh manifest files and usr/bin
                    var ret = AppsModel.deleteApp(app_id)
                    if(ret)
                    {
                        console.log("removed: "+app_id + " name: "+name + " app_index: "+index)
                        listModel.remove(index, 1)
                        AppsModel.set_current_index((AppsModel.get_current_index()-1))
                    }
                    else
                    {
                        error_msg.text = "Error while deleting " + name + " application"
                        error_msg.visible = true
                    }


                }
                onNo: {
                    deleteMsg.visible=false
                }
      }
      MouseArea{
            anchors.fill: parent
            //propagateComposedEvents: true
            enabled: true
            z:1
            onPressed: {
                _gradient .opacity = 1
                mouse.accepted = true
                console.log("pressed");
                root.clicktime1 = new Date().valueOf()
            }
            onReleased: {
                console.log("released");

                root.clicktime2 = new Date().valueOf()
                if((root.clicktime2-root.clicktime1) > 200){
                    if(root.isDeleteMode === false)
                    {
                        busyIndication2.visible = true
                        root.t1 = new Date().valueOf()
                        var ret = -1
                        appDelegate.GridView.view.currentIndex = index
                        if ((t1-t2) > 500){
                            ret = AppsModel.query_startapp(app_id)
                            if(ret === -1)
                            {
                                error_msg.setText("Reached permitted number of live apps.")
                                error_msg.visible=true
                            }
                            else if(ret === -2)
                            {
                                error_msg.setText("App id:"+app_id+" is not valid!")
                                error_msg.visible=true
                            }
                            else if(ret === -3)
                            {
                                error_msg.setText("Invalid hash value")
                                error_msg.visible=true
                            }
                            else if(ret === 0)//successful
                            {
                                console.log("appname: " + name)
                                 if(app_id === 0) //if appstore reload list
                                {
                                    content.reload()
                                }
                            }
                            else
                            {
                                error_msg.setText("Unknown error occured.")
                                error_msg.visible=true
                            }
                        }
                        root.t2 = new Date().valueOf()
                        busyIndication2.visible = false
                    }
                    else //delete mode
                    {
                        if(name === "APPSTORE")
                        {

                        }
                        else
                        {
                          deleteMsg.visible=true
                        }
                    }
              }
              _gradient .opacity = 0.45
            }
      }
    }
}
