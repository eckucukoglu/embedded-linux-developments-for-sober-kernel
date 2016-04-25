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
            text: "Filesystem Encryption"

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
                    filesystemLoc.text = "";
                    pwDialog.encrypting = !(pwDialog.encrypting)
                    if(checked) //encrypt
                    {
                        enOrDeCryptedText.text = "Filesystem to be encrypted: "
                        encryptFileNamesCheckBox.opacity = 1.0
                        recognizeMeCheckBox.opacity = 1.0
                        //fileDialog.setNameFilters("Any filesystem (*)")
                    }
                    else{
                        enOrDeCryptedText.text = "Filesystem to be decrypted: "
                        encryptFileNamesCheckBox.opacity = 0.0
                        recognizeMeCheckBox.opacity = 0.0
                        //fileDialog.setNameFilters("Any filesystem (*)")

                    }
                }

            }

            Text{
                text: " a filesystem"
                anchors.verticalCenter: parent.verticalCenter
            }
        }


        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                id: enOrDeCryptedText
                text: "Filesystem to be decrypted: "
                anchors.verticalCenter: parent.verticalCenter
            }

            Text{
                id: filesystemLoc
                anchors.verticalCenter: parent.verticalCenter
                width: 400
                text: ""
                elide: Text.ElideRight
                font.underline: true
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
                selectFolder: true
                onAccepted: {
                    filesystemLoc.text = encDecHandler.getLocalFilePath(fileDialog.fileUrl)
                }
            }
        }

        CheckBox{
            id: encryptFileNamesCheckBox
            text: "I also want to encrypt file names in that directory"
            opacity: 0.0

            Behavior on opacity{ NumberAnimation{easing.type: Easing.OutQuint}}
        }
        CheckBox{
            id:recognizeMeCheckBox
            text: "Recognize me"
            opacity: 0.0
            Behavior on opacity{ NumberAnimation{easing.type: Easing.OutQuint}}
        }

        Button{
            text: "Apply"
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked:{

                if(filesystemLoc.text.length != 0)
                    pwDialog.open()
                else{
                    console.log("Enter a valid file")
                }

            }
        }

        PasswordDialog{
            id: pwDialog

            onPasswordEntered: {    //In a PasswordDialog, we know that the filesystem is valid.
                //In this slot, we know that password is also provided
                //by the user.
                var result;
                if(encDecSwitch.checked){
                    //we will encrypt the fileystem
                    if(encryptFileNamesCheckBox.checkedState == Qt.Checked){
                        //Encrpyt with encrypt file names.
                        if(recognizeMeCheckBox.checkedState == Qt.Checked)
                            result = encDecHandler.initiateFilesystemEncryption(filesystemLoc.text, password, true, true)
                        else{
                            result = encDecHandler.initiateFilesystemEncryption(filesystemLoc.text, password, true, false)
                        }
                    }
                    else{
                        if(recognizeMeCheckBox.checkedState == Qt.Checked)
                            result = encDecHandler.initiateFilesystemEncryption(filesystemLoc.text, password, false, true)
                        else{
                            result = encDecHandler.initiateFilesystemEncryption(filesystemLoc.text, password, false, false)
                        }
                    }

                    if(result == false)
                        failedDialog.open();
                    else{
                        doneDialog.open();
                    }
                }
                else{
                    //we will decrypt the file

                    result = encDecHandler.initiateFilesystemDecryption(filesystemLoc.text, password)
                    if(result == false)
                        failedDialog.open();
                    else{
                        doneDialog.open();
                    }
                }

                refreshFilepathList();

            }
            Component.onCompleted:{
                encrypting: encDecSwitch.checked

            }

            function refreshFilepathList(){
                encryptedFSListModel.clear();
                var pathList = encDecHandler.getListOfEncryptedFilesystemNames()
                if(pathList){
                    for(var i = 0; i < pathList.length; i++)
                    {
                        encryptedFSListModel.append({path: pathList[i]});
                    }
                }
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

        ListView{
            id: encryptedFSListView
            model: encryptedFSListModel
            width: 400
            height: 300

            header: Component{
                id: headerOfView
                Item{
                    width: 300
                    height: 20
                    Text{
                        text: "The List of Encrypted Filesystems"
                        font.pixelSize: 18
                        font.underline: true
                    }
                }
            }

            delegate: Component {
                id: contactDelegate
                Item {
                    width: 300; height: 40
                    Rectangle{
                        anchors.fill: parent
                        border.width: 3
                        border.color: "lightsteelblue"
                        Text { text: '<b>Path:</b>\t ' + path;
                            font.pixelSize: 14;
                            anchors.centerIn: parent}
                    }

                }
            }
        }

        ListModel{
            id:encryptedFSListModel
        }
    }


}

