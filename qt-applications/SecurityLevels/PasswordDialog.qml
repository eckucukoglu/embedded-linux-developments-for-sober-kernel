import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

Item{
    property bool encrypting
    signal open

    signal passwordEntered(string password)
    signal dialogCancelled

    Dialog{
        id: passDialog
        height: 150
        width: 450
        title: "Enter password"
        standardButtons: StandardButton.Apply | StandardButton.Cancel

        modality:  Qt.WindowModal

        Text{
            id: infoText
            text: "Enter your key to " + (encrypting ? "ENCRYPT" : "DECRYPT") + " your file/filesystem"
        }

        TextField {
            id: passwordBox
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: infoText.bottom
            anchors.topMargin: 20
            maximumLength: 8

            placeholderText: "password"
            echoMode: TextInput.Password
            onAccepted:{
                passwordBox.text = ""

            }
        }

        Component.onCompleted: {
            // let user type in password without tapping on
            // the text field
            passwordBox.forceActiveFocus();
        }

        onApply: {
            //console.log(passwordBox.text.length)
            if(passwordBox.text.length > 0){
                passDialog.close()
                passwordEntered(passwordBox.text);
                passwordBox.text = "";
            }
            else{
                passDialog.open()
            }
        }

        //Warning Dialog when password is invalid
        MessageDialog {
            id: invalidPassDialog
            title: "Invalid Password"
            text: "Please enter a valid password"
            standardButtons: StandardButton.Ok
            icon: StandardIcon.Warning
            width: 400
            height: 200
        }

        onRejected: {
            passwordBox.text = "";
            passDialog.close();
            dialogCancelled()
        }

    }
    onOpen: {
        passDialog.open()
    }
}


