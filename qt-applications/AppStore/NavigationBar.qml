import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item{
    width: parent.width
    height:24
    anchors.bottom: parent.bottom
        Image {
            id: home_icon
            source: "pics/sober_newspecs/icon/homeIcon.png"
            x: (parent.width/2)-16
            scale: 0.7
            z:1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
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
            color: "black"
            opacity: 0.4
            anchors.fill: parent
        }
}
