// global variables

var SECURITY_RETURN_TYPE = {
    SECURITY_RETURN_OK : 0,
    ERR_SECURITY_RETURN_NOK : 1,
    ERR_SECURITY_SHAMIR_NUMB_OF_THRESHOLD_NOT_REACHED : 2,
    ERR_SECURITY_SHAMIR_SERVER_NOT_REACHABLE : 3,
    ERR_SECURITY_DISC_ENC_SALT_FILE_NOT_EXIST : 4,
    ERR_SECURITY_DISC_ENC_SALT_LENGTH_ERROR : 5,
    ERR_SECURITY_DISC_RESET : 6,
    ERR_HTTP_REQUEST_ERROR : 7
};

var LOGINHELPER_ERR_TYPE = {
    LOGINHELPER_RET_ERROR : -1,
    LOGINHELPER_RET_OK : 0,
};

var resultMsg  = ""


// functions
function clearInfoArea(){
    infoTextArea.textvalue  = qsTr("Enter Password")
    numbersContent.counter = 0
    numbersContent.password = qsTr("")
}

function isInitMode() {
    initMode = loginHelper.get_initMode()
    if(initMode===LOGINHELPER_ERR_TYPE.LOGINHELPER_RET_ERROR)
     {
         messageDialog.text= "Init Mode cannot be read"
         messageDialog.visible=true
         initMode = 0
     }
     else{
     }
     if(initMode === 1)
     {
         chckBoxArea.visible=false
     }
     console.log("initmode: "+initMode)
     return initMode
}
function func_initMode(){
    console.log("intializing disc..")
    var res =loginHelper.initDisc(qsTr(numbersContent.password), numbersContent.isShamir)
    resultMsg = "shamir server connection is successful"
    if(res === SECURITY_RETURN_TYPE.SECURITY_RETURN_OK)
    {
        loginHelper.set_initMode(1, numbersContent.isShamir);
        chckBoxArea.visible=false //hide shamir checkbox
        initMode=1 //initmode is off
        console.log("init mode: "+initMode)
        infoTextArea.textvalue  = qsTr("Enter Password..")
        numbersContent.counter = 0
        numbersContent.password = qsTr("")
        console.log("loading desktop")
        loginHelper.query_login(0)
        console.log("loaded desktop")
        clearInfoArea()
    }
    else{
        check_errors(res)
    }
    console.log(resultMsg)
}

function checkPassword(){
    var result = loginHelper.check_password(qsTr(numbersContent.password), numbersContent.isShamir)
    console.log("return from checkpassword: " + result)
    if(result === SECURITY_RETURN_TYPE.SECURITY_RETURN_OK){ //valid password
        numbersContent.trialRemaining=5
        loginHelper.set_attemptCount(0)
        waitTime=10
        console.log("loading desktop")
        loginHelper.query_login(0)
        console.log("loaded desktop")
    }
   else{ //incorrect password or connection error
        if(result === SECURITY_RETURN_TYPE.ERR_SECURITY_RETURN_NOK) //incorrect password
        {
            numbersContent.trialRemaining = 5 - (loginHelper.get_attemptCount()%5)
            console.log("remaining trial: "+numbersContent.trialRemaining)
            console.log("attempt count: "+loginHelper.get_attemptCount())
            if( loginHelper.get_attemptCount()!= 0 && (loginHelper.get_attemptCount()%5) === 0)
            {
                   ftime = waitTime
                   numbersContent.enabled=false
                   chckBoxArea.enabled=false
                   numbersContent.trialRemaining=5
                   deviceLockArea.visible=true
                   lockTimer.running = true
            }
            else{
                  numbersContent.enabled=false
                  chckBoxArea.enabled=false
                  errorMsg.visible=true
            }
            loginHelper.set_attemptCount(loginHelper.get_attemptCount() + 1)
        }
        else //connection error
        {
            check_errors(result)
        }
    }
}


function check_errors(res){
    if(res === SECURITY_RETURN_TYPE.ERR_HTTP_REQUEST_ERROR)
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: HTTP request error\n"
        messageDialog.text = "Internet Connection Error!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_RETURN_NOK)
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: Security return is NOT OK\n"
        messageDialog.text = "Password is incorrect!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_RETURN_NOK)
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: Security return is NOT OK\n"
        messageDialog.text = "Password is incorrect!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_SHAMIR_SERVER_NOT_REACHABLE)

    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: passed: HTTP request OK\n"
        resultMsg += "qml: failed: Shamir server is not reachable"
        messageDialog.text = "Server Connection Error!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_SHAMIR_NUMB_OF_THRESHOLD_NOT_REACHED)
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: passed: HTTP request OK\n"
        resultMsg += "qml: passed: Shamir server is reachable"
        resultMsg += "qml: failed: Shamir number of Threshold is not reached"
        messageDialog.text = "Enough Server Pairs Not Reachable!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_DISC_ENC_SALT_FILE_NOT_EXIST)
    {
         resultMsg  = "initialize failed:\n"
         resultMsg += "qml: failed: initialize failed: Disc Encyription salt file doesnot exist"
         messageDialog.text = "Internal Error! Missing salt file!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_DISC_ENC_SALT_LENGTH_ERROR)
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: Disc encyription salt length error"
        messageDialog.text = "Internal Error! Salt value incorrect!"
    }
    else if(res === SECURITY_RETURN_TYPE.ERR_SECURITY_DISC_RESET) /********/
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: Disc Reset Failed"
        messageDialog.text = "Internal Error!"
    }
    else
    {
        resultMsg  = "initialize failed:\n"
        resultMsg += "qml: failed: unknown error\n"
        messageDialog.text = "Unknown Error!"
    }
    messageDialog.visible = true
}

