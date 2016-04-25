import QtQuick 2.0

Item{
    property alias text: textarea.text
    property alias font: textarea.font
    property alias color: textarea.color
    property alias horizontalAlignment: textarea.horizontalAlignment
    property alias verticalAlignment: textarea.verticalAlignment
    Text{
        id: textarea
        font.family: "Helvetica"
        font.weight: Font.Light
        color: "#d5d1d1"
        anchors.fill: parent
    }
}
