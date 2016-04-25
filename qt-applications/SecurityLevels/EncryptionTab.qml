import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2

Item {
    anchors.fill: parent
    anchors.bottom: parent.bottom
    anchors.topMargin: 20
        Text{
            id: titleText
            text: "Encryption Settings"
            anchors.horizontalCenter: parent.horizontalCenter
        }

        TabView{
            anchors.top: titleText.bottom
            anchors.topMargin: 20
            anchors.fill: parent
            Tab{
                title: "File Encrypt/Decrypt"
                FileEncDecTab{

                }
            }

            Tab{
                title: "Filesystem Encrypt/Decrypt"
                FilesystemEncDecTab{

                }
            }
        }
}

