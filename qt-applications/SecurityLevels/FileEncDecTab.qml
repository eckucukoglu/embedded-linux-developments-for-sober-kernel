import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import EncDecHandler 1.0

Item {
    anchors.fill: parent
    anchors.topMargin: 20

    EncDecHandler{
        id: encDecHandler
    }

    Column{
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Text{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "File Encryption"

        }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            spacing: 10
            Text{
                id: iWantToText
                text: "I want to "
                anchors.verticalCenter: parent.verticalCenter

            }
            Switch{
                id: encDecSwitch
                style: CustomSwitchStyle{
                    choice1: "Encrypt"
                    choice2: "Decrypt"
                }

                onCheckedChanged: {
                    pwDialog.encrypting = !(pwDialog.encrypting)
                    fileLoc.text = "";
                    if(checked) //encrypt
                    {
                        fileToBeEncDecText.text = "File to be encrypted: "
                        fileDialog.setNameFilters("All files (*)")
                    }
                    else{
                        fileToBeEncDecText.text = "File to be decrypted: "
                        fileDialog.setNameFilters("Encrypted files (*.enc)")
                    }
                }
            }

            Text{
                text: " a file"
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                id: fileToBeEncDecText
                text: "File to be decrypted: "
                anchors.verticalCenter: parent.verticalCenter
            }

            Text{
                id: fileLoc
                anchors.verticalCenter: parent.verticalCenter
                width: 400
                font.underline: true
                text: ""
                elide: Text.ElideRight
            }

            Button{
                text: "Browse.."
                onClicked:{
                    fileDialog.visible = true
                }
            }

            FileDialog{
                id: fileDialog
                title: "Please choose a file"
                folder: shortcuts.home
                onAccepted: {
                    fileLoc.text = encDecHandler.getLocalFilePath(fileDialog.fileUrl)
                    console.log("You chose: " + fileLoc.text)
                }
                onRejected: {
                    console.log("Canceled")

                }
            }
        }

        Button{
            text: "Apply"
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked:{

                if(fileLoc.text.length != 0)
                    pwDialog.open()
                else{
                    console.log("Enter a valid file")
                }

            }
        }

        PasswordDialog{
            id: pwDialog

            onPasswordEntered: {    //In a PasswordDialog, we know that the file is valid.
                //In this slot, we know that password is also provided
                //by the user.
                var result;
                if(encDecSwitch.checked){
                    //we will encrypt the file
                    result = encDecHandler.initiateFileEncryption(fileLoc.text, password)
                    if(result == false)
                        failedDialog.open();
                    else{
                        doneDialog.open();
                    }
                }
                else{
                    //we will decrypt the file
                    result = encDecHandler.initiateFileDecryption(fileLoc.text, password)
                    if(result == false)
                        failedDialog.open();
                    else{
                        doneDialog.open();
                    }
                }

            }

            Component.onCompleted:{
                encrypting: encDecSwitch.checked
                if(!encDecSwitch.checked)
                    fileDialog.setNameFilters("Encrypted files (*.enc)");
            }
        }

        MessageDialog{
            id: failedDialog
            text: "Operation Failed."
            icon: StandardIcon.Critical
        }
        MessageDialog{
            id: doneDialog
            text: "Operation Done."
            icon: StandardIcon.Information
        }
    }
}

