import QtQuick 2.0
import QtQuick.Controls 1.4
import SELinuxHandler 1.0

Item {
    anchors.fill: parent
    anchors.topMargin: 20

    SELinuxHandler{
        id: seLinuxHandler
    }

    Column{
        spacing: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Text{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Change SELinux Mode"

        }

        Text{
            id: currentModeText
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Current Mode : " + (seLinuxHandler.getStatus())
        }

        Row{
            anchors.horizontalCenter: parent.horizontalCenter
            Text{
                text: "I want my SELinuxMode to be : "
                verticalAlignment: Text.AlignVCenter
                anchors.verticalCenter: parent.verticalCenter
            }

            Switch{
                id: permissiveEnforcedSwitch
                style: CustomSwitchStyle{
                    choice1: "Enforcing"
                    choice2: "Permissive"

                }
            }
        }

        Text{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "ENFORCING: The strict mode where your access control is heavily restricted. The SELinux policies are enforced."
            color: "#007700"
        }

        Text{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "PERMISSIVE: The policies are not enforced but SELinux logs what it would have blocked or granted."
            color: "#003c77"
        }


        Button{
            id: applyButton
            text: "Apply"
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked:{

                if(permissiveEnforcedSwitch.checked){
                    console.log("Switch to Enforcing!")
                    seLinuxHandler.setMode(1)
                }
                else{
                    console.log("Switch to Permissive!")
                    seLinuxHandler.setMode(0)
                }

                refreshStatus();
            }

            function refreshStatus(){
                currentModeText.text = "Current Mode : " + (seLinuxHandler.getStatus());
            }
        }
    }
}

