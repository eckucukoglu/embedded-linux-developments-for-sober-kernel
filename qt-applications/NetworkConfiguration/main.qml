import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Network Configuration")
    width: 800
    height: 480
    visible: true
    property string currentIP
    property string currentGateway
    property string currentNetmask
    property string currentDNS
    property string currentMAC

    ApplicationArea{
        id: applicationArea
        anchors.fill: parent
        z:1


        LeftWidgetArea{
            id: leftWidgetArea
            x:0
            y:0
            z:1
            title: "NETWORK SETTINGS"
            description: "You can manage your network"
            iconSource: "assets/network_icon.png"

            anchors.top: parent.top
            anchors.bottom: parent.bottom


        }

        Keyboard{
            id: keyboard
            x: 160
            y: 160
            z: 100
        }

    }

}
