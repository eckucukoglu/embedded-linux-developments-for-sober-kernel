import QtQuick 2.0

Item{
    property string info
    property alias text: textInput.text

    width: 587
    height: 33

    SoberText{
        id: infoText
        text: info
        font.pixelSize: 20
        width: 110
        height: 20
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle{
        id: addressRect
        anchors.verticalCenter: parent.verticalCenter
        color: "#4E4D55"
        anchors.left: infoText.right
        anchors.leftMargin: 100

        width: 286
        height: 33

        TextInput{
            id: textInput
            color: readOnly ? "#858181" : "#d5d1d1"
            font.pixelSize: 20
            font.family: "Helvetica"
            width: 273

            anchors.centerIn: parent
            readOnly: true
            text: "HH:HH:HH:HH:HH:HH"
        }
    }
}
