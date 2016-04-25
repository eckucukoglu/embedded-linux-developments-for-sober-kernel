import QtQuick 2.0

Rectangle{
    height: 24
    width: parent.width
    anchors.top: parent.top
    color: "transparent"
    property var date
    Rectangle{
        anchors.fill: parent
        color: "black"
        opacity: 0.3
    }
    Text{
        id: hourAndDate
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 20
        text: date.toLocaleTimeString(Qt.locale("en_EN"), "hh:mm:ss")
        font.pixelSize: 12
        color: "white"
    }
    Timer{
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            date = new Date();
            hourAndDate.text = date.toLocaleTimeString(Qt.locale("tr_TR"), "hh:mm:ss")
        }
    }
    Text{
        id: appName
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: "SOBERSTORE"
        font.pixelSize: 14
        font.bold: true
        color: "white"
    }
    Component.onCompleted: {
        date = new Date();
    }
}
