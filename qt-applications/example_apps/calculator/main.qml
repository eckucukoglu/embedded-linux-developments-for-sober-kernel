import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1


ApplicationWindow{
    width: 800
    height: 480
    id: root
    visible: true

    Rectangle{
        width: parent.width
        anchors.bottomMargin: navigationBar.height
        anchors.fill: parent
        id: gridHolder
        GridLayout{
            Button {
                    id: button1
                    x: 378
                    y: 332
                    width: 194
                    height: 66
                    text: qsTr("Button")
                }



        }

    }

    NavigationBar{
        id: navigationBar
    }

    Component.onCompleted: {

    }
}





