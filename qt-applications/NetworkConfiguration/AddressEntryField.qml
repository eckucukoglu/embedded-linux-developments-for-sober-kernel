import QtQuick 2.0

Item{
    id: root
    property string info
    property alias text: textInput.text
    property alias readOnly: textInput.readOnly
    signal textEdited(string txt)

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

            onTextChanged: {
                root.textEdited(text);
                console.log(text)
            }


            inputMask:  "000.000.000.000;"
//            validator:RegExpValidator{
//            regExp:/^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
//            }

        }

    }

}

