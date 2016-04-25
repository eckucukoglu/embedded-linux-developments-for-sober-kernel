function updateProcessesList(){
    statsString = cpuMemHandler.readAllStatFiles().split("\n");
    //var name, mem, pid;

    //clear the list
    while(processListModel.count > 0){
        processListModel.remove(0);
        //processListModel.sync()
    }

    //var memHumanReadable
    //var processEntry

    for(var i = 0; i < statsString.length; i++){
        processEntry = statsString[i].split(" ")

        name = processEntry[0]
        mem = parseInt(processEntry[1])
        pid = parseInt(processEntry[3])

        if(mem < 1024){
            memHumanReadable = mem + " KB"
        }
        else{
            memHumanReadable = ((mem / 1024).toFixed(2)) + " MB"
        }

        //now add this entry to our model
        processListModel.insert(0, {name: name, memory: memHumanReadable, pid: pid, cpu: "12"})
    }
    gc()
}


function refreshCpuAndRamValues(){
    //Get ram percentage and update ram progress bar
    ramPerc = cpuMemHandler.getRamPercentage();
    deviceMemory.value = ramPerc

    //update total percentage
    cpuPercentages[0] = cpuMemHandler.getCpuPercentage(0);
    cpuPercentagesHumanReadable[0] = (cpuPercentages[0] * 100).toFixed(0);
    //totalCpuUsage.value = cpuPercentages[0];

    //update individual cpu percentages
    for(var i = 1; i <= numberOfCpus; i++){
        cpuPercentages[i] = cpuMemHandler.getCpuPercentage(i);
        cpuPercentagesHumanReadable[i] = (cpuPercentages[i] * 100).toFixed(0);
        cpus.itemAt(i-1).value = cpuPercentages[i];
    }

    //refresh the progress bar texts:
    //if the value is below 6%, don't write it down to the progress bar since it won't be seen
    totalCpuUsage.progressBarText = (cpuPercentagesHumanReadable[0] < 6) ? "" : (cpuPercentagesHumanReadable[0]   + " %");
    totalCpuUsage.bgText = (100 - cpuPercentagesHumanReadable[0]) + " %"
    for(var l = 1; l <= numberOfCpus; l++){
        cpus.itemAt(l - 1).progressBarText = (cpuPercentagesHumanReadable[l] < 6) ? "" : (cpuPercentagesHumanReadable[l]   + " %");
        cpus.itemAt(l - 1).bgText = (100 - cpuPercentagesHumanReadable[l]) + " %"
    }
    gc()
}
