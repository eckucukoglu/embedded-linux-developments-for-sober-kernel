import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3

Item{
    property alias text: title.text
    property alias value: pBar.value
    property int progressBarRightMargin
    property string fillColor: pBar.color
    property string bgText
    property string progressBarText

    width: 300
    height: 50

    SoberText{
        width: parent.width
        anchors.top: parent.top
        id: title
        font.pixelSize: 20
        height: 20
    }

    ProgressBar{
        id: pBar
        style: progressBarStyle
        anchors.top: title.bottom
        anchors.topMargin: 15
        //value: 0.4
        width: 300
        height: 15

        Behavior on value {NumberAnimation{easing.type: Easing.OutCubic}}
    }

    Component{
        id:progressBarStyle
        ProgressBarStyle{
            background: Rectangle {
                color: "#4e4c55"

                SoberText{
                    horizontalAlignment: Qt.AlignRight
                    anchors.right: parent.right
                    anchors.rightMargin: progressBarRightMargin
                    anchors.top: parent.top
                    anchors.topMargin: -3
                    //anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    text: bgText
                }
            }
            progress: Rectangle {
                color: fillColor
                //border.color: "steelblue"

                SoberText{
                    horizontalAlignment: Qt.AlignRight
                    anchors.right: parent.right
                    anchors.rightMargin: progressBarRightMargin
                    anchors.top: parent.top
                    anchors.topMargin: -3
                    //anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: 16
                    font.weight: Font.Normal
                    text: progressBarText
                    color: "#444547"
                }
            }

        }
    }

}
