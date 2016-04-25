import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import CpuMemHandler 1.0

Component{
    id:processDelegate

    Item{
        property int currentIndex: 0
        id:root
        width: 430
        height: 45
        onCurrentIndexChanged: {
            slide_anim.to = - content.width * currentIndex + 400
            slide_anim.start()
            slide_anim.alwaysRunToEnd = true
        }

        states: [
            State{
                name: "selected"
                PropertyChanges{
                    target: contentContainer
                    color: "#44666666"
                }
            },
            State{
                name: "dropped"
                PropertyChanges{
                    target: contentContainer
                    color: "transparent"
                }
            }
        ]


        SwipeArea {
            anchors.fill: parent
            id:swipeArea
            onMove: {
                root.state = "selected"
                if((x > 0 && currentIndex != 0) || (x < 0 && currentIndex != numberOfPages-1 )){ //only when swipeable
                    content.x =(-content.width * currentIndex) + x + 400
                }
            }
            onSwipe: {
                root.state = "dropped"
                console.log("swipe.")
                switch (direction) {
                case "left":
                    if (currentIndex === numberOfPages - 1) {
                        currentIndexChanged()
                    }
                    else {
                        currentIndex++
                        memText.opacity = 0
                        killProcessButton.opacity = 1
                        cancelButton.opacity = 1
                        //currentIndexChanged()
                    }
                    break
                case "right":
                    if (currentIndex === 0) {
                        currentIndexChanged()   //realign the view
                    }
                    else {
                        currentIndex--          //change the index
                        memText.opacity = 1
                        killProcessButton.opacity = 0
                        cancelButton.opacity = 0
                        //currentIndexChanged()   //then realign the view
                    }
                    break
                }

            }

            onCanceled: {
                contentContainer.color = "transparent"
                currentIndexChanged()
            }

        }

        Rectangle{
            id: contentContainer
            anchors.fill: parent
            color: "transparent"
            border.width: 1
            border.color: "#4e4c55"
            PropertyAnimation {
                id: slide_anim
                target: content
                //easing.type: Easing.Linear

                properties: "x"
                duration: 300
            }

            Behavior on color{
                ColorAnimation{
                    easing.type: Easing.OutQuad
                }
            }


            Rectangle{
                id: coloredCircle
                width: 20
                height: 20
                radius: width * 0.5
                border.color: Qt.rgba(Math.random(), Math.random(), Math.random(), 1)
                color: "transparent"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 8

                Rectangle{
                    anchors.fill: parent
                    radius: parent.width * 0.5
                    color: parent.border.color
                    opacity: 0.4
                }
            }

            SoberText{
                id: processName
                anchors.left: coloredCircle.right
                //anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 10
                anchors.top: parent.top
                anchors.topMargin: 10
                font.pixelSize: 16

                text: name
            }

            Item{
                id: content
                width: 440
                x: contentContainer.x + 400
                anchors.bottom: parent.bottom
                anchors.top: parent.top
                //color: "transparent"

               SoberText{
                    id: memText
                    horizontalAlignment: Qt.AlignRight
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    font.pixelSize: 16

                    text: memory
                    Behavior on opacity {NumberAnimation{easing.type: Easing.OutCurve; duration: 300}}
                }



               SoberText{
                   id: cpuUsageText
                   anchors.top: parent.top
                   anchors.topMargin: 10
                   anchors.right: memText.right
                   font.pixelSize: 16

                   text: ""
                   Behavior on opacity {NumberAnimation{easing.type: Easing.OutCurve; duration: 300}}
               }
        }

        }
        CpuMemHandler{
            id: cpuMemHandler
        }

        //        Component.onCompleted: {
        //            contentLoader.sourceComponent = contentComponent
        //        }

    }



}
