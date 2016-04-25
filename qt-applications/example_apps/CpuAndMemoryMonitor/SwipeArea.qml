/* This code was written by Sergejs Kovrovs and has been placed in the public domain. */

import QtQuick 2.0

MouseArea {
    property point origin
    property bool ready: false
    signal move(int x, int y)
    signal swipe(string direction)
    propagateComposedEvents: true

    onPressed: {
        console.log("swipe area on pressed")
        //mouse.accepted = true
        propagateComposedEvents = true

        drag.axis = Drag.XAndYAxis
        origin = Qt.point(mouse.x, mouse.y)

    }

    onPositionChanged: {
        //console.log("mouse.x :" + mouse.x +" and origin.x: " + origin.x)
        switch (drag.axis) {
        case Drag.XAndYAxis:
            if (mouse.y != origin.y) {
                //console.log("axis changed to Y axis")
                drag.axis = Drag.YAxis
            }
            if (mouse.x != origin.x) {
                //console.log("axis changed to X axis")
                drag.axis = Drag.XAxis
            }
            break
        case Drag.XAxis:
            move(mouse.x - origin.x, 0)
            break
        case Drag.YAxis:
            //move(0, mouse.y - origin.y)
            break
        }
    }

    onReleased: {
        var swiped = false;
        //propagateComposedEvents = false;

        console.log("swipe area on released")
        switch (drag.axis) {
        case Drag.XAndYAxis:
            drag.axis = Drag.YAxis
            break
        case Drag.XAxis:
            if(origin.x - mouse.x > 100){
                swipe("left")
                swiped = true
            }
            else if(mouse.x - origin.x > 100){
                swipe("right")
                swiped = true
            }
            else{
                canceled(mouse)
            }
            break
        case Drag.YAxis:
            break
        }

        if(swiped){
            propagateComposedEvents = false;
            mouse.accepted = true;

        }
    }
}
