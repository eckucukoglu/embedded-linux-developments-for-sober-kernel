import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3
import FileIO 1.0


ApplicationWindow {
    title: qsTr("CPU and RAM Monitor")
    width: 1024
    height: 600
    visible: true


    RamMonitor{
        id: ramMonitor
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left

    }

    Rectangle{
        id: border1
        width: parent.width
        height: 1
        color: "black"
        anchors.top: ramMonitor.bottom
    }

    CpuMonitor{
        id: cpuMonitor
        anchors.top: border1.bottom
        anchors.right: parent.right
        anchors.left: parent.left

    }

    Rectangle{
        id: border2
        width: parent.width
        height: 1
        color: "black"
        anchors.top: cpuMonitor.bottom
    }

    Processes{
        id:processes
        width: parent.width
        anchors.top: border2.bottom
        anchors.bottom: parent.bottom
    }
}
