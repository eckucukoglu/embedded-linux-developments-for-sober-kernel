import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.0


    Rectangle {
        id: infoTextArea
        width: 240
        height: 60
        color: "transparent"
        property var textvalue: qsTr("Enter Password..")

        Column {
            width: parent.width
            height: parent.height
            Rectangle{
                id: overlay
                width: parent.width
                height: parent.height
                border.width: 2
                border.color: "lightblue"
                color: "transparent"
                radius: width * 0.5
                Rectangle{
                    width: parent.width
                    height: parent.height
                    color: "lightblue"
                    opacity: 0.2
                    radius: parent.radius
                }

                Text {
                    id: infoText
                    text: textvalue
                    width: parent.width
                    height: parent.height
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 22
                    font.family: "Helvetica"
                    color: "white"
                }
            }

       }
    }






