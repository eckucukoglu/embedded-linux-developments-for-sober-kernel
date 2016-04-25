/* This code was written by Sergejs Kovrovs and has been placed in the public domain. */

import QtQuick 2.0


MouseArea {
    property point origin
    property bool ready: false
    signal move(int x, int y)
    signal swipe(string direction)
    propagateComposedEvents: true
    onPressed: {
        mouse.accepted = true
        propagateComposedEvents = true

        drag.axis = Drag.XAxis
        origin = Qt.point(mouse.x, mouse.y)

    }
    onPositionChanged: {
        switch (drag.axis) {
        case Drag.XAndYAxis:
            if (Math.abs(mouse.x - origin.x) > 0) {
                drag.axis = Drag.XAxis
            }
            else if (Math.abs(mouse.y - origin.y) > 0) {
                drag.axis = Drag.YAxis
            }
            break
        case Drag.XAxis:
            move(mouse.x - origin.x, 0)
            break
        case Drag.YAxis:
            move(0, mouse.y - origin.y)
            break
        }
    }
    onReleased: {
        var swiped = false;
        switch (drag.axis) {
        case Drag.XAndYAxis:
            canceled(mouse)
            break
        case Drag.XAxis:
            if(origin.x - mouse.x > 200){
                swipe("left")
                swiped = true
            }
            else if(mouse.x - origin.x > 200){
                swipe("right")
                swiped = true
            }
            else{
                canceled(mouse)
            }
            break
        case Drag.YAxis:
            swipe(mouse.y - origin.y < 0 ? "up" : "down")
            break
        }

        if(swiped){
            propagateComposedEvents = false;
            mouse.accepted = true;
        }
    }
}
