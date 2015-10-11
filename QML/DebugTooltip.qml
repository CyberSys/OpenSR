import QtQuick 2.3
import OpenSR 1.0
import OpenSR.World 1.0

Item {
    property WorldObject object
    
    Rectangle {
        anchors.fill: info
        color: "#FFFFFF"
    }
    
    Text {
        id: info
        width: 400
        wrapMode: Text.Wrap
    }
    
    onObjectChanged: {
        var text = "";
        for (var key in object) {
            if (key == "objectName" || key == "children")
                continue;

            if(typeof object[key] == "function")
                continue;

            var value = object[key];

            if (key == "typeId") {
                key = "type";
                value = WorldManager.typeName(value)
            }

            text += "<b>" + key + "</b>" + ": " + value + "<br/>\n";
        }
        info.text = text;
    }
} 
