import QtQuick 2.0
import QtQuick.Dialogs 1.2
import NetworkHandler 1.0

Item{
    Rectangle {
        anchors.fill: parent
        color: "#444347"

        Column{
            id: switchesColumn
            x: 280
            y: 20
            spacing: 10
            SwitchField{
                id: wirelessEthernetSwitchField

                info: "Connection Type:"
                text2: "Wireless"
                text1: "Ethernet"

                onSwitched: {
                    wirelessEthernetSwitchField.activeText = (wirelessEthernetSwitchField.activeText == text1) ? (text2) : (text1)
                    refreshEntryList(activeText);
                    console.log("refreshEntryList done.");
                }

                function refreshEntryList(activeText){
                    currentMAC = networkHandler.getMACAddress(activeText)
                    currentIP = networkHandler.getIP(activeText);
                    currentNetmask = networkHandler.getNetmask(activeText)
                    currentGateway = networkHandler.getGateway(currentIP, currentNetmask)
                    console.log("getGateway done for." + activeText);

                    ipEntryField.text = currentIP;
                    netmaskEntryField.text = currentNetmask;
                    gatewayEntryField.text = currentGateway;
                    macAddressField.text = currentMAC;
                }
            }


            SwitchField{
                id: manualAutoSwitchField

                info: "Network Configuration:"
                text1: "Auto"
                text2: "Manual"

                onSwitched: {
                    manualAutoSwitchField.activeText = (manualAutoSwitchField.activeText == text1) ? (text2) : (text1)

                    ipEntryField.readOnly = !ipEntryField.readOnly
                    dnsEntryField.readOnly = !dnsEntryField.readOnly
                    netmaskEntryField.readOnly = !netmaskEntryField.readOnly
                    gatewayEntryField.readOnly = !gatewayEntryField.readOnly
                }
            }
        }


        Column{
            id: addressEntriesColumn
            x:280
            y:140
            spacing: 10
            z:2
            AddressEntryField{
                id: ipEntryField
                z:1
                info: "IP Address:"
                readOnly: true
                onTextEdited: {
                    currentIP = text;
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        keyboard.activeText = "ipEntryField"
                        keyboard.info = currentIP
                        console.log((manualAutoSwitchField.activeText == "Manual") ? keyboard.visible=true : keyboard.visible=false)
                    }

                }

            }

            AddressEntryField{
                id: netmaskEntryField
                z:1
                info: "Netmask:"
                readOnly: true
                onTextEdited: {
                    currentNetmask = netmaskEntryField.text;
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        keyboard.activeText = "netmaskEntryField"
                        keyboard.info = currentNetmask
                        console.log((manualAutoSwitchField.activeText == "Manual") ? keyboard.visible=true : keyboard.visible=false)
                    }
                }
            }

            AddressEntryField{
                id: gatewayEntryField
                z:1
                info: "Gateway Address:"
                readOnly: true
                text: "1 1 1"
                onTextEdited: {
                    currentGateway = gatewayEntryField.text;
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        keyboard.activeText = "gatewayEntryField"
                        keyboard.info = currentGateway
                        console.log((manualAutoSwitchField.activeText == "Manual") ? keyboard.visible=true : keyboard.visible=false)
                    }
                }
            }

            AddressEntryField{
                id: dnsEntryField
                z:1
                info: "DNS Address:"
                readOnly: true

                onTextEdited: {
                    currentDNS = dnsEntryField.text;
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        keyboard.activeText = "dnsEntryField"
                        keyboard.info = currentDNS
                        console.log((manualAutoSwitchField.activeText == "Manual") ? keyboard.visible=true : keyboard.visible=false)
                    }
                }
            }
        }

        Column{
            id: macAddressColumn
            x:280
            y:370
            z:0
            spacing: 10

            MacField{
                id: macAddressField
                info: "MAC Address"
            }
        }


        Row{
            x:525
            y:420
            spacing: 20
            z:1
            CustomButton{
                id: checkButton
                text: "Check"

                property bool upAndRunning

                onClicked: {
                    upAndRunning = networkHandler.checkConnection(wirelessEthernetSwitchField.activeText);

                    //**************************************************
                    //Up and running check events will be inserted here.
                    //**************************************************
                    popUpCheckResultDialog();
                }

                function popUpCheckResultDialog(){
                    checkResultDialog.setVisible(true)
                }
            }

            CustomButton{
                id: applyButton
                text: "Apply"

                onClicked:{
                    areYouSureDialog.details = areYouSureDialog.prepareDetailsText();
                    popUpAreYouSureScreen();

                }
                function popUpAreYouSureScreen(){
                    areYouSureDialog.setVisible(true)

                }
            }
        }
    }

    MessageDialog{
        property string details: prepareDetailsText()

        id: areYouSureDialog
        title: "Are You Sure?"
        icon: StandardIcon.Question
        text: "These changes might corrupt your network settings."
        informativeText: (manualAutoSwitchField.activeText == "Manual") ? "Use <b>Auto</b> to revert back any changes." : ""
        detailedText: details
        standardButtons: StandardButton.Yes | StandardButton.No
        onYes: {loadingScreen.setVisible(true);
            networkHandler.applyConfiguration(wirelessEthernetSwitchField.activeText, manualAutoSwitchField.activeText, currentIP, currentNetmask, currentGateway, currentDNS)
            loadingScreen.setVisible(false);
        }
        onVisibleChanged: details = prepareDetailsText();
        modality: Qt.WindowModal


        function prepareDetailsText(){
            var str = "";
            if(manualAutoSwitchField.activeText == "Auto")
                str += "The network settings will be automatically configured for the " + wirelessEthernetSwitchField.activeText.toLowerCase() + " interface \n" +
                        "DNS settings will be also reset.";
            else{
                str += "The following changes will be made:\n";
                str += "IP Address: " + currentIP + "\n";
                str += "Netmask: " + currentNetmask + "\n";
                str += "Gateway Address: " + currentGateway + "\n";

                if(dnsEntryField.text != "..."){    //three dots that are separators
                    str +=  "Also, the following DNS address will be set as your main DNS server: " + currentDNS;

                }
            }

            return str
        }
    }

    MessageDialog{
        id: checkResultDialog
        title: "Checking Your Network Status.."
        icon: StandardIcon.Information
        text: "The interface " + wirelessEthernetSwitchField.activeText.toUpperCase() + " is: \n\n" +
              (checkButton.upAndRunning ? "UP AND RUNNING" : "DOWN");
        modality: Qt.WindowModal
    }

    MessageDialog{
        id: loadingScreen
        modality: Qt.WindowModal
        text: "LOADING.\nPLEASE WAIT..."
        title: "Loading..."
    }

    NetworkHandler{
        id: networkHandler
    }



    Component.onCompleted: {

        //Filling the blanks initially.
        wirelessEthernetSwitchField.activeText = "Ethernet"
        console.log(wirelessEthernetSwitchField.activeText)
        currentMAC = networkHandler.getMACAddress(wirelessEthernetSwitchField.activeText)
        currentIP = networkHandler.getIP(wirelessEthernetSwitchField.activeText);
        currentNetmask = networkHandler.getNetmask(wirelessEthernetSwitchField.activeText)
        console.log("calling getGateway for ethernet.");
        currentGateway = networkHandler.getGateway(currentIP, currentNetmask)
        console.log("getGateway done for ethernet.");

        ipEntryField.text = currentIP;
        netmaskEntryField.text = currentNetmask;
        gatewayEntryField.text = currentGateway;
        macAddressField.text = currentMAC;
    }


    function refreshFields(){
        ipEntryField.text = currentIP
        netmaskEntryField.text = currentNetmask
        gatewayEntryField.text = currentGateway
        dnsEntryField.text = currentDNS
    }
}



