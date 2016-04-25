import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.0

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
        text: date.toLocaleTimeString(Qt.locale("en_EN"), "hh:mm:ss") + "  |  " +
              date.toLocaleDateString(Qt.locale("en_EN"), "dd MMM yyyy")
        font.pixelSize: 12
        color: "white"
    }

    MessageDialog {
        id: msgLogout
        title: "Warning!"
        visible: false
        icon: StandardIcon.Warning
        text: "You are logging out. Do you want to continue?"
        standardButtons: StandardButton.Yes |  StandardButton.No
        onYes: console.log("yes")
        onNo: console.log("no")
    }


    Row{
        id: rowItems
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.rightMargin: 20
        spacing: 8
        Image{
            id: energyIcon
            source: "pics/sober_newspecs/icon/icon-battery.png"
            scale: 0.9
        }

        Image{
            source: "pics/sober_newspecs/icon/icon-settings.png"
            scale: 0.7
            Rectangle{
                anchors.top: root.top
                anchors.horizontalCenter: parent.horizontalCenter
                width: 120
                height: 50
                opacity: 0
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        settingsMenu.visible ^=true
                    }
                }
            }
        }

    }


   Rectangle{
        id: settingsMenu
        visible: false
        anchors.top: statusBar.bottom
        anchors.right: parent.right
        width: 180
        height: 120
        radius: 6

        Image{
            source: root.backgroundImg
            anchors.fill: parent
            opacity: 0.28
        }
        Rectangle{
            id: verticalSeperator
            width: 2
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0.4
            x: 42
            color: "black"
            height: parent.height-10
        }

        Rectangle{
             id: lockBtn
             anchors.top: parent.top
             width: parent.width
             color: "transparent"
             height: 40
             Image{
                source: "pics/lock.png"
                scale: 0.18
                anchors.right: lockTxt.left
                anchors.rightMargin: -28
                anchors.verticalCenter: parent.verticalCenter

             }
             Text{
                 id: lockTxt
                 text: "Lock"
                 anchors.right: parent.right
                 anchors.verticalCenter: parent.verticalCenter
                 anchors.rightMargin: 80
                 font.bold: true
                 font.pixelSize: 18
                 font.family: "helvetica"
                 color: "black"
             }
             RadialGradient {
                 id: gradientLock
                 visible: false
                 anchors.fill: parent
                 horizontalRadius: parent.width -10
                 verticalRadius: 84
                 gradient: Gradient {
                     GradientStop { position: 0.0; color: "#E6EBED" }
                     GradientStop { position: 0.5; color: "transparent" }
                 }
             }
             MouseArea{
                 anchors.fill: parent
                 propagateComposedEvents: false
                 onClicked: {
                       settingsMenu.visible=false
                       AppsModel._securityResetDiscEncryption()
                       console.log("check")
                       AppsModel.query_lockscreen()
                 }
             }
        }
        Rectangle{
            anchors.top: lockBtn.bottom
            width: parent.width - 10
            color: "#A8A8A8"
            anchors.horizontalCenter: parent.horizontalCenter
            height: 2
        }
        Rectangle{
             id: powerOffBtn
             anchors.top: lockBtn.bottom
             width: parent.width
             color: "transparent"
             height: 40
             Image{
                source: "pics/poweroff.png"
                scale: 0.4
                anchors.right: powerOffTxt.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 2
                opacity: 0.8

             }
             Text{
                 id: powerOffTxt
                 text: "Power Off"
                 anchors.right: parent.right
                 anchors.verticalCenter: parent.verticalCenter
                 anchors.rightMargin: 40
                 font.bold: true
                 font.pixelSize: 18
                 font.family: "helvetica"
                 color: "black"
             }
             RadialGradient {
                 id: gradientPwrOff
                 visible: false
                 anchors.fill: parent
                 horizontalRadius: parent.width -10
                 verticalRadius: 84
                 gradient: Gradient {
                     GradientStop { position: 0.0; color: "#E6EBED" }
                     GradientStop { position: 0.5; color: "transparent" }
                 }
             }
             MessageDialog {
                 id: powerOffMsg
                 title: "Warning!"
                 icon: StandardIcon.Question
                 text: "Do you want to Power Off"
                 standardButtons: StandardButton.Yes | StandardButton.No
                 onYes: {
                     AppsModel._securityResetDiscEncryption()
                     AppsModel.shutdown()
                 }

                 onNo: {
                     console.log("canceled")
                     powerOffMsg.visible=false
                 }
             }

             MouseArea{
                 anchors.fill: parent
                 propagateComposedEvents: false
                 onClicked: {
                       settingsMenu.visible=false
                       powerOffMsg.visible=true
                 }
             }
        }

        Rectangle{
            anchors.top: powerOffBtn.bottom
            width: parent.width - 10
            color: "#A8A8A8"
            anchors.horizontalCenter: parent.horizontalCenter
            height: 2
        }

        Rectangle{
             id: deleteBtn
             anchors.top: powerOffBtn.bottom
             width: parent.width
             color: "transparent"
             height: 40
             Image{
                source: "pics/sober_newspecs/icon/icon_remove.png"
                scale: 0.4
                anchors.right: deleteTxt.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.rightMargin: 2
                opacity: 0.8
             }
             Text{
                 id: deleteTxt
                 text: "Delete Apps"
                 anchors.right: parent.right
                 anchors.verticalCenter: parent.verticalCenter
                 anchors.rightMargin: 20
                 font.bold: true
                 font.pixelSize: 18
                 font.family: "helvetica"
                 color: "black"
             }
             MouseArea{
                 anchors.fill: parent
                 propagateComposedEvents: false
                 onClicked: {
                       settingsMenu.visible=false
                       root.isDeleteMode = true
                 }
             }
        }

    }
    Timer{
        interval: 100
        running: true
        repeat: true
        onTriggered: {
            date = new Date();
            hourAndDate.text = date.toLocaleTimeString(Qt.locale("tr_TR"), "hh:mm:ss") + "  |  " +
                      date.toLocaleDateString(Qt.locale("tr_TR"), "dd MMM yyyy")
        }
    }

    Component.onCompleted: {
        date = new Date();
    }
}
