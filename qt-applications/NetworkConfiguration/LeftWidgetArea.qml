import QtQuick 2.0

Item {
    width: 250
    height: 480

    property alias title: titleText.text
    property alias description: descText.text
    property alias iconSource: img.source

    Rectangle{
        anchors.fill: parent
        color: "#3b393e"

        Image{
            id: img
            anchors.top: parent.top
            anchors.topMargin: 80
            anchors.horizontalCenter: parent.horizontalCenter
        }

        SoberText{
            id:titleText
            anchors.top: img.bottom
            anchors.topMargin: 40
            font.pixelSize: 22
            horizontalAlignment: Qt.AlignHCenter
            anchors.right: parent.right
            anchors.left: parent.left
        }

        SoberText{
            id:descText
            anchors.top: titleText.bottom
            anchors.topMargin: 40
            font.pixelSize: 14
            horizontalAlignment: Qt.AlignHCenter
            anchors.right: parent.right
            anchors.left: parent.left
        }

        Rectangle{
            id:greenRightStroke
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 1
            color: "#0b988b"
        }

        CustomButton{
            id: quitBtn
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 40


            text: "Quit"
            onClicked: {
              Qt.quit()
            }
        }
    }

}

