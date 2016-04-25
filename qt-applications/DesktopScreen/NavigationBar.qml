import QtQuick 2.0
import QtQuick.Dialogs 1.2

Item{
    width: parent.width
    height:24
    //color: "transparent"
    anchors.bottom: parent.bottom

    Rectangle{
        anchors.fill: parent
        color: "black"
        opacity: 0.2
    }

    MessageDialog{
        id: savedMsg
        visible: false
        text: "Changes Saved!"
        onAccepted: savedMsg.visible=false
    }

    MessageDialog {
              id: error_msg2
              title: "Error"
              visible:false
              icon: StandardIcon.Warning
              text: ""
              onAccepted: visible = false
    }

    Rectangle{
            id: deleteExitBtn
            visible: root.isDeleteMode
            anchors.horizontalCenter: parent.horizontalCenter
            width: 60
            height: parent.height - 4
            color: "transparent"
            border.width: 1
            border.color: "white"
            radius: 10
            Rectangle{
                anchors.fill: parent
                color: "white"
                opacity: 0.2
            }

            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: "EXIT"
                color: "white"
                font.bold: true
                font.family: "Helvetica"

            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    root.isDeleteMode=false
                }
            }

        }
}
