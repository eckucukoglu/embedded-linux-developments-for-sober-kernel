import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("CPU and Memory Monitor")
    width: 800
    height: 480
    visible: true


   //for delegates
    property int numberOfPages: 2

    ApplicationArea{
        LeftWidgetArea{
            title: "Memory Monitor"
            description: "You can monitor your Memory Usage"
            iconSource: "assets/icon_cpu.png"
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
    }

}
