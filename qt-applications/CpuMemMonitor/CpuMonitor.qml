import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.3
import FileIO 1.0

Item{
    property alias cpu0progressBar: cpu0pbar
    property alias cpu0percentage: cpu0perc
    property alias cpu1progressBar: cpu1pbar
    property alias cpu1percentage: cpu1perc


    width: 1024
    height: 80

    Column{
        width: parent.width
        height: parent.height

        Rectangle{
            width: parent.width
            height: parent.height / 2

            Text{
                text: "CPU 0"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 30
            }

            ProgressBar{
                id:cpu0pbar
                value: 0.04
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                style: progressBarStyleBlue
                Behavior on value{NumberAnimation{duration: 100}}
            }

            Text{
                id:cpu0perc
                text: "4%"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 30
            }

            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onEntered: parent.color = "#97B9F7"
                onExited: parent.color = "white"
            }

        }

        Rectangle{
            width: parent.width
            height: parent.height / 2

            Text{
                text: "CPU 1"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 30
            }

            ProgressBar{
                id:cpu1pbar
                value: 0.1
                anchors.centerIn: parent
                anchors.verticalCenter: parent.verticalCenter
                style: progressBarStyleBlue
                Behavior on value{NumberAnimation{duration: 100}}
            }

            Text{
                id:cpu1perc
                text: "10%"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 30

            }

            MouseArea{
                hoverEnabled: true
                anchors.fill: parent
                onEntered: parent.color = "#97B9F7"
                onExited: parent.color = "white"
            }

        }
    }

    Component{
        id:progressBarStyleBlue
        ProgressBarStyle{
            background: Rectangle {
                        radius: 2
                        color: "lightgray"
                        border.color: "gray"
                        border.width: 1
                        implicitWidth: 500
                        implicitHeight: 30
                    }
                    progress: Rectangle {
                        color: "lightsteelblue"
                        border.color: "steelblue"
                    }

        }
    }



    FileIO{
        id: procstatFile
        source: "/proc/stat"
        onError: console.log(msg)
    }

    //Variables for cpu stats
    property int prevtotal0:0
    property int total0:0
    property int prevtotal1:0
    property int total1:0
    property int previdle0:0
    property int idle0:0
    property int previdle1:0
    property int idle1:0

    function getOverallCpuStats(){
        //Works for two cpus (cpu0 and cpu1 only)

        //Assign old values to prev. totals
        prevtotal0 = total0;
        previdle0 = idle0;
        prevtotal1 = total1;
        previdle1 = idle1;

        var content = procstatFile.read();
        var lines = content.split('\n');
        delete content
        var cpu0stats = lines[1].split(" ");
        var cpu1stats = lines[2].split(" ");

        //Getting idle0 and idle1 values:
        idle0 = parseInt(cpu0stats[4]);
        idle1 = parseInt(cpu1stats[4]);

        //calculate total:
        total0 = 0
        total1 = 0
        for(var i = 1; i < 8; i++){
            total0 += parseInt(cpu0stats[i])
            total1 += parseInt(cpu1stats[i])
        }

        //Calculate the differences, then percentage of cpu working
        var cpu0PercentageWorking = ((total0 - prevtotal0) - (idle0 - previdle0)) / (total0 - prevtotal0)
        var cpu1PercentageWorking = ((total1 - prevtotal1) - (idle1 - previdle1)) / (total1 - prevtotal1)

        cpu0percentage.text = (cpu0PercentageWorking * 100).toFixed(2) + " %"
        cpu1percentage.text = (cpu1PercentageWorking * 100).toFixed(2) + " %"
        cpu0progressBar.value = cpu0PercentageWorking
        cpu1progressBar.value = cpu1PercentageWorking

    }

    Component.onCompleted: { getOverallCpuStats();gc();
                            }

    Timer{
        id:refresher
        interval: 3000
        running: true
        repeat: true

        onTriggered: {
            getOverallCpuStats()
        }
    }
}

