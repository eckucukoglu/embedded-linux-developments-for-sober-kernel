import QtQuick 2.0
import QtQuick.Controls 1.3
import CpuMemHandler 1.0
import "helperfunctions.js" as Logic

Item{
    property int numberOfCpus: cpuMemHandler.getNumberOfCpus();
    property var cpuColors: ["#fce7c2", "#f68d76", "#679911", "#f17711", "#42aa48", "#d64222", "#229977", "#1caaee"]
    property double ramPerc
    property int totalRam
    property var cpuPercentages: []
    property var cpuPercentagesHumanReadable: []


    CpuMemHandler{
        id:cpuMemHandler
    }

    anchors.fill: parent


    Rectangle{
        anchors.fill: parent
        color: "#444347"

        Flickable{
            x:390
            y:70
            width: 440
            height: 480
            contentWidth: contentColumn.width
            contentHeight: contentColumn.height
            flickableDirection: Flickable.VerticalFlick
            //boundsBehavior: Flickable.OvershootBounds //at Qt5.5

            Column{
                id: contentColumn
                spacing: 50


              CustomProgressBar{
                    id:totalCpuUsage
                    width: 440
                    height: 20
                    anchors.left: parent.left
                    anchors.leftMargin: -50
                    value: cpuPercentages[0]
                    text: "CPU Usage"
                    fillColor: "#80808f"
                    progressBarText: "0 %"
                    bgText: "100 %"
                    progressBarRightMargin: 3
                }

                Repeater{
                    id: cpus
                    model:numberOfCpus
                    CustomProgressBar{
                        width: 440
                        height: 20
                        anchors.left: parent.left
                        anchors.leftMargin: -50
                        value: cpuPercentages[index + 1]
                        text: "Core " + index
                        fillColor: cpuColors[index % cpuColors.length]
                        progressBarRightMargin: 3
                        progressBarText: "0 %"
                        bgText: "100 %"
                    }
                }

                CustomProgressBar{
                    id:deviceMemory
                    width: 440
                    height: 20
                    anchors.left: parent.left
                    anchors.leftMargin: -50
                    value: ramPerc
                    text: "Device Memory"
                    fillColor: "#04caad"
                    progressBarText: (totalRam * ramPerc).toFixed()  + " MB"
                    bgText: (totalRam - totalRam * ramPerc).toFixed() + " MB"
                    progressBarRightMargin: 3
                }

                Loader{
                    id: processesLoader
                    width: 300
                }

           }
        }
    }


    Component.onCompleted: {
        processesLoader.source = "Processes.qml"
        //******************************************
        //update cpu values
        //******************************************
        cpuMemHandler.updateCpuValues();
        for(var j = 0; j <= numberOfCpus; j++){
            cpuPercentages.push(cpuMemHandler.getCpuPercentage(j));
            cpuPercentagesHumanReadable.push((cpuPercentages[j] * 100).toFixed(0));
        }

        //initially, the values of the cpus are all zeroes.
        //totalCpuUsage.value = 0;
        for(var i = 0; i < numberOfCpus; i++){
            cpus.itemAt(i).value = 0;
        }

        //******************************************
        //update ram values
        //******************************************
        totalRam = cpuMemHandler.getTotalRam();
        //console.log(totalRam)
        Logic.refreshCpuAndRamValues();
    }


    Timer{
        id:refresher
        interval: 3000
        running: true
        repeat: true

        onTriggered: {
            cpuMemHandler.updateCpuValues();
            Logic.refreshCpuAndRamValues();
            gc()
        }
    }
}
