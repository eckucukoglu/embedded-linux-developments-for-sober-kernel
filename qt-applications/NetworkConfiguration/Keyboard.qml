import QtQuick 2.5

Rectangle {
        width: 288
        height: 206
        color: "transparent"
        visible: false
        property string info: "0.0.0.0"
        property string activeText: ""

        Rectangle {
              id: keyboardBackground
              anchors.fill: parent
              implicitWidth: 96
              implicitHeight: 44
              border.color: "#0b988b"
              border.width: 2
              color: "#444347"

              Rectangle{
                  anchors.fill: parent
                  color:"white"
                  opacity: 0.1
              }
        }

        Rectangle{
            id: infoTxtArea
            anchors.top: parent.top
            anchors.left: parent.left
            width: parent.width
            color: "transparent"
            height: 30

            TextInput{
                id: infoTxt
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                text: info
                color: "white"
                font.pixelSize: 22

                //inputMask:  "000 . 000 . 000 . 000;"
            }
        }

        CustomButton{
            id: btn1
            text: "1"
            anchors.top: infoTxtArea.bottom
            anchors.left: parent.left
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "1")
            }
        }
        CustomButton{
            id: btn2
            text: "2"
            anchors.left: btn1.right
            anchors.top: infoTxtArea.bottom
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "2")
            }
        }
        CustomButton{
            id: btn3
            text: "3"
            anchors.left: btn2.right
            anchors.top: infoTxtArea.bottom
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "3")
            }
        }
        CustomButton{
            id: btn4
            text: "4"
            anchors.top: btn1.bottom
            anchors.left: parent.left
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "4")
            }
        }
        CustomButton{
            id: btn5
            text: "5"
            anchors.top: btn2.bottom
            anchors.left: btn4.right
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "5")
            }
        }
        CustomButton{
            id: btn6
            text: "6"
            anchors.top: btn3.bottom
            anchors.left: btn5.right
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "6")
            }
        }
        CustomButton{
            id: btn7
            text: "7"
            anchors.top: btn4.bottom
            anchors.left: parent.left
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "7")
            }
        }
        CustomButton{
            id: btn8
            text: "8"
            anchors.top: btn5.bottom
            anchors.left: btn7.right
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "8")
            }
        }
        CustomButton{
            id: btn9
            text: "9"
            anchors.top: btn6.bottom
            anchors.left: btn8.right
            onClicked:{
                 infoTxt.insert(infoTxt.cursorPosition, "9")
            }
        }
        CustomButton{
            id: btnClear
            text: "<--"
            anchors.top: btn7.bottom
            anchors.left: parent.left
            onClicked:{
                infoTxt.remove(infoTxt.cursorPosition-1, infoTxt.cursorPosition);
            }
        }
        CustomButton{
            id: btn0
            text: "0"
            anchors.top: btn8.bottom
            anchors.left: btnClear.right
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, "0")
            }
        }
        CustomButton{
            id: btnClose
            text: "Close"
            anchors.top: btn9.bottom
            anchors.left: btn0.right
            onClicked:{
                keyboard.visible=false
                switch(activeText)
                {
                     case "ipEntryField":
                        currentIP = infoTxt.text;
                     break;
                     case "netmaskEntryField":
                         currentNetmask = infoTxt.text;
                     break;
                     case "gatewayEntryField":
                         currentGateway = infoTxt.text;
                     break;
                     case "dnsEntryField":
                         currentDNS = infoTxt.text;
                     break;
                }
                applicationArea.refreshFields()
            }
        }
        CustomButton{
            id: btnEmpty
            text: ""
            enabled: false
            anchors.top: btnClear.bottom
            anchors.left: parent.left
        }
        CustomButton{
            id: btnDot
            text: "."
            anchors.top: btn0.bottom
            anchors.left: btnEmpty.right
            onClicked:{
                infoTxt.insert(infoTxt.cursorPosition, ".")
            }
        }
        CustomButton{
            id: btnEmpty2
            text: ""
            anchors.top: btnClose.bottom
            anchors.left: btnDot.right
            enabled: false
        }

}
