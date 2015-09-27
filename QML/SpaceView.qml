import QtQuick 2.3
import OpenSR 1.0

Item {
    property string bgImage: "res:/DATA/BGObj/bg00.gai"
    property int speed: 500
    property int bgSpeed: 10
    property int scrollSize: 10
    property int maxScrollTime: 600

    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "black"
    }


    Item {
        x: parent.width / 2
        y: parent.height / 2

        Image {
            id: bg
            source: bgImage
            x: -width / 2
            y: -height / 2
        }

        Item {
            id: spaceNode
            Text {
                color: "red"
                x: -width / 2
                y: -height / 2
                font.pixelSize: 100
                text: "System\ncontent"
            }
        }
    }

    ParallelAnimation {
        id: hAnim
        PropertyAnimation {
            id: hBgAnim
            target: bg
            property: "x"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
        PropertyAnimation {
            id: hFgAnim
            target: spaceNode
            property: "x"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
    }
    ParallelAnimation {
        id: vAnim
        PropertyAnimation {
            id: vBgAnim
            target: bg
            property: "y"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
        PropertyAnimation {
            id: vFgAnim
            target: spaceNode
            property: "y"
            duration: maxScrollTime * 1000
            alwaysRunToEnd: false
        }
    }

    MouseArea {
        id: leftHoverArea

        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        anchors.topMargin: scrollSize
        anchors.bottomMargin: scrollSize

        width: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed;
            hFgAnim.to = spaceNode.x + maxScrollTime * speed;
            hAnim.start()
        }
        onExited: { hAnim.stop() }
    }
    MouseArea {
        id: rightHoverArea

        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        anchors.topMargin: scrollSize
        anchors.bottomMargin: scrollSize

        width: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed;
            hFgAnim.to = spaceNode.x - maxScrollTime * speed;
            hAnim.start()
        }
        onExited: { hAnim.stop() }
    }
    MouseArea {
        id: topHoverArea

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: scrollSize
        anchors.rightMargin: scrollSize

        height: scrollSize

        hoverEnabled: true
        onEntered: {
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed;
            vFgAnim.to = spaceNode.y + maxScrollTime * speed;
            vAnim.start()
        }
        onExited: { vAnim.stop() }
    }
    MouseArea {
        id: bottomHoverArea

        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: scrollSize
        anchors.rightMargin: scrollSize

        height: scrollSize

        hoverEnabled: true
        onEntered: {
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed;
            vFgAnim.to = spaceNode.y - maxScrollTime * speed;
            vAnim.start()
        }
        onExited: { vAnim.stop() }
    }
    MouseArea {
        id: topleftHoverArea

        anchors.left: parent.left
        anchors.top: parent.top

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x + maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y + maxScrollTime * speed / Math.sqrt(2);
            hAnim.start()
            vAnim.start()
        }
        onExited: { hAnim.stop(); vAnim.stop() }
    }
    MouseArea {
        id: toprightHoverArea

        anchors.right: parent.right
        anchors.top: parent.top

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x - maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y + maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y + maxScrollTime * speed / Math.sqrt(2);
            hAnim.start()
            vAnim.start()
        }
        onExited: { hAnim.stop(); vAnim.stop() }
    }
    MouseArea {
        id: bottomleftHoverArea

        anchors.left: parent.left
        anchors.bottom: parent.bottom

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x + maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x + maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y - maxScrollTime * speed / Math.sqrt(2);
            hAnim.start()
            vAnim.start()
        }
        onExited: { hAnim.stop(); vAnim.stop() }
    }
    MouseArea {
        id: bottomrightHoverArea

        anchors.right: parent.right
        anchors.bottom: parent.bottom

        width: scrollSize
        height: scrollSize

        hoverEnabled: true
        onEntered: {
            hBgAnim.to = bg.x - maxScrollTime * bgSpeed / Math.sqrt(2);
            hFgAnim.to = spaceNode.x - maxScrollTime * speed / Math.sqrt(2);
            vBgAnim.to = bg.y - maxScrollTime * bgSpeed / Math.sqrt(2);
            vFgAnim.to = spaceNode.y - maxScrollTime * speed / Math.sqrt(2);
            hAnim.start()
            vAnim.start()
        }
        onExited: { hAnim.stop(); vAnim.stop() }
    }
} 
