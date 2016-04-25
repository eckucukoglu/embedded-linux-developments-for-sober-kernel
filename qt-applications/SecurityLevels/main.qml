import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

ApplicationWindow {
    title: qsTr("Security Configuration")
    width: 1024
    height: 600
    visible: true

    TabView{
        anchors.fill: parent
        Tab{
            title: "Access Control"
            SELinuxTab{

            }
        }
        Tab{
            title: "Encryption/Decryption"
            EncryptionTab{

            }
        }
    }
}
