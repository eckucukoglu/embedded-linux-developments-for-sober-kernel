import QtQuick 2.5
import QtGraphicalEffects 1.0
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.0
import "functions.js" as FUNCTIONS

Component {
    id: buttonDelegate
    Rectangle {
        id: buttonDelegateRect
        width: 60
        height: 60
        color: "transparent"
        x:0
        Column {
            width: parent.width
            height: parent.height

            Rectangle{
                id: overlay
                width: parent.width
                height: parent.height
                radius: width * 0.5
                border.width: 2
                border.color: borderColor
                color: "transparent"

                Rectangle{
                    id: _gradient
                    width: parent.width
                    height: parent.height
                    radius: parent.radius
                    color: "transparent"
                    opacity: 0.3

                    RadialGradient {
                        anchors.fill: parent
                        horizontalRadius: 60
                        verticalRadius: 60
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: borderColor }
                            GradientStop { position: 0.5; color: "transparent" }
                        }
                    }
                }
                Text {
                    id: buttonName
                    text: name
                    width: parent.width + 6
                    elide: Text.ElideRight
                    anchors.horizontalCenter: parent.horizontalCenter
                    horizontalAlignment: Text.AlignHCenter
                    anchors.verticalCenter:  parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: textsize
                    font.family: "Helvetica"
                    color: "white"
                }
            }
        }


      MouseArea{
            anchors.fill: parent
            propagateComposedEvents: true

            onPressed: {
                mouse.accepted = true
                _gradient.opacity = 1
            }
            onReleased: {
                 _gradient.opacity = 0.3
            }
            onClicked: {
                buttonDelegate.GridView.view.currentIndex = index
                console.log("counter: "+numbersContent.counter)
                if(index == 9) //Cancel Button
                {
                       FUNCTIONS.clearInfoArea()
                }
                else if(index == 11) //OK Button
                {

                    initMode = FUNCTIONS.isInitMode()
                    console.log("initmode: "+initMode)
                    if(initMode === 1) //not initialize mode
                    {
                        FUNCTIONS.checkPassword()
                        FUNCTIONS.clearInfoArea()
                    }
                    else{ //initialize mode
                        FUNCTIONS.func_initMode()
                    }

                }
                else //DIGIT
                {
                    if(numbersContent.counter == 0)
                    {
                        infoTextArea.textvalue = qsTr("")
                    }
                    else
                    {

                    }
                    if(numbersContent.counter < 10) //pwd must be less than 10 digits
                    {
                        infoTextArea.textvalue = qsTr(infoTextArea.textvalue) + qsTr("*")
                        numbersContent.counter++
                        numbersContent.password = numbersContent.password + qsTr(name)
                    }
                    else
                    {

                    }
                }
            }
        }
    }
}

