import QtQuick 2.0
import QtQuick.Controls.Styles 1.2

SwitchStyle{
    id: customSwitchStyle
    property string choice1
    property string choice2
    groove: Rectangle{
        implicitWidth: 300
        implicitHeight: 40
        Rectangle{
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            width: parent.width/2 - 2
            anchors.margins: 2
            color: control.checked ? "#007700" : "#222222"
            Behavior on color {ColorAnimation {duration: 750}}

            Text {
                font.pixelSize: 23
                color: "white"
                anchors.centerIn: parent
                text: choice1
            }
        }
        Rectangle{
            width: parent.width/2
            height: parent.height
            anchors.right: parent.right
            color: control.checked ?"#222222" :"#003C77"
            Behavior on color {ColorAnimation {duration: 750}}

            Text{
                anchors.centerIn: parent
                text: choice2
                color: "white"
                font.pixelSize: 23
            }
        }
        color: "#222"
        border.color: "#444"
        border.width: 2

    }

    handle: Rectangle {
        width: parent.parent.width/2
        height: control.height
        color: "#444"
        border.color: "#555"
        border.width: 2
    }
}
