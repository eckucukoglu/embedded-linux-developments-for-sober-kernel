import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls.Styles 1.3
import Qt.labs.folderlistmodel 2.1
import FileIO 1.0
import SortFilterProxyModel 0.1

Item{
    width:1024
    height: 400
    Rectangle{
        id: processesTitle
        width: parent.width
        height: 60
        z:10

        Image{
            id: arrow1
            source: "arrow-up.png"
            rotation: 180
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 20
            scale: 0.3
            Behavior on rotation {RotationAnimation{duration: 300}}
        }

        Image{
            id: arrow2
            source: "arrow-up.png"
            rotation: 180
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 20
            scale: 0.3
            Behavior on rotation {RotationAnimation{duration: 300}}
        }

        Text{
            text: "PROCESSES"
            anchors.centerIn: parent
            font.pixelSize: 24
        }

        MouseArea{
            anchors.fill: parent
            hoverEnabled:true
            onClicked: {if(processListView.opacity == 0) processListView.opacity = 1;
                else    processListView.opacity = 0;
                arrow1.rotation += 180; arrow2.rotation += 180}

            onEntered: parent.color = "#97B9F7";
            onExited: parent.color = "white";
        }
    }

    ListModel{
        id: processListModel
        property bool isRefreshing: false
        //Create list elements of 'processes' from proc/pid/'s

    }

    TableView{
        id: processListView
        width: parent.width
        height: 350

        sortIndicatorVisible: true
        sortIndicatorColumn: 3
        sortIndicatorOrder: Qt.DescendingOrder
        ;

        alternatingRowColors: true

        TableViewColumn{
            role: "pid"
            title: "Process ID"
            width: (parent.width/ 4) - 22
        }

        TableViewColumn{
            role: "name"
            title: "Name"
            width: parent.width/ 4
        }

        TableViewColumn{
            role: "mem"
            title: "Memory Usage (MB)"
            width: parent.width/4
        }

        TableViewColumn{
            role: "memUsage"
            title: "Memory Usage (%)"
            width: parent.width/4
        }

        opacity: 1
        Behavior on opacity{ NumberAnimation{duration: 500; easing.type: Easing.OutCubic} }
        clip: true
        anchors.top: processesTitle.bottom
        anchors.bottom: killProcessBar.top
        //rowDelegate: processDelegate


        //FOCUS HERe
        //FOCUS HEER
        //FOCUS HERe
        //FOCUS HEER
        //FOCUS HERe
        //FOCUS HEER
        //FOCUS HERe
        //FOCUS HEER

        itemDelegate: Item{
            Text {
                anchors.verticalCenter: parent.verticalCenter
                color:  styleData.textColor
                elide: styleData.elideMode
                text: "  " + styleData.value
            }
            MouseArea{
                onClicked:{processListView.selection.select(index)}
            }


        }
        style: TableViewStyle{
        }

        model: processListModel

//        model: SortFilterProxyModel{
//            id: proxyModel
//            source: processListModel.count > 0 ? processListModel : null
//            sortOrder: processListView.sortIndicatorOrder
//            sortCaseSensitivity: Qt.CaseInsensitive
//            sortRole: processListView.getColumn(processListView.sortIndicatorColumn).role
//        }
    }

    Component{
        id: processDelegate

        Item{
            width: parent.width
            height: 50


            Rectangle{
                id: topBorder
                width: parent.width
                height: 1
                color: "blue"
            }

            Rectangle{
                id: botBorder
                width: parent.width
                height: 1
                color: "blue"
                //anchors.top: contentRect.bottom
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {styleData.selected = true}
            }
        }
    }

    Component.onCompleted: {
        processListModel.isRefreshing = true;
        fillProcessList();
        processListModel.isRefreshing = false;
    }

    function fillProcessList(){
        processListModel.isRefreshing = true

        //Get total RAM stats from meminfo file.
        var content = meminfoFile.read();
        var splitted = content.split("\n");
        var ramTotalStat = splitted[0].split(/\s+/);
        ramTotalStat = (parseInt(ramTotalStat[1]) / 1024).toFixed()

        //First, clear the model.
        processListModel.clear();
        processListView.selection.clear();

        //Get stats about all processes from proc pid status
        var processedStats = statReader.readAllStatFiles();
        var processedStatList = processedStats.split("\n");
        var nameTemp, pidTemp, memTemp, memPerc
        var memTempHumanReadable
        var processEntry

        for(var i = 0; i < processedStatList.length; i++){

            processEntry = processedStatList[i].split(" ");

            nameTemp = processEntry[0]
            pidTemp = parseInt(processEntry[1])
            memTemp = (parseInt(processEntry[2]) / 1024).toPrecision(3)
            memPerc = parseFloat(((memTemp / ramTotalStat) * 100).toFixed(2))


            if(parseFloat(memTemp) < 1){
                memTempHumanReadable = parseFloat(memTemp) * 1024;
                memTempHumanReadable = memTempHumanReadable.toPrecision(3);
                memTempHumanReadable += " KB"
            }
            else{
                memTempHumanReadable = memTemp + " MB"
            }

            //Add the parsed pid name and memory values to our list model
            processListModel.append({pid: pidTemp, name: nameTemp,
                                        mem: memTempHumanReadable, memUsage: memPerc })
            processListView.update();

        }

        processListModel.isRefreshing = false
        gc();
    }

    FileIO{
        id: statReader
    }

    FileIO{
        id: meminfoFile
        source: "/proc/meminfo"
        onError: console.log(msg)
    }

    Timer{
        id:refresher
        interval: 5000
        running: true
        repeat: true

        onTriggered: {
            fillProcessList()
        }
    }


    Rectangle{
        id: killProcessBar
        width: parent.width
        height: 60
        anchors.bottom: parent.bottom

        Button{
            text: "KILL PROCESS"
            anchors.centerIn: parent

            style: ButtonStyle{
                background: Rectangle{
                    implicitWidth: 200
                    implicitHeight: 50
                    border.width: control.pressed ? 2 : 1
                    border.color: "#4479DB"
                    radius: 4

                    color: control.pressed ? "#97B9F7" : "white"

                }
            }

            onClicked:{
                if(processListView.selection.count != 0){
                    var pid = processListModel.get(processListView.currentRow).pid
                    console.log(pid)
                    statReader.tryToKillProcess(pid);
                }

            }

        }

    }
}


