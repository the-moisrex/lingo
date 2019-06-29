import QtQuick 2.9

Item {
    property alias hoverEnabled: mouse.hoverEnabled
    property color hoveredColor: "#ddd"
    property alias text: icon.text
    property alias font: icon.font

    width: icon.implicitWidth * 2
    height: icon.implicitHeight * 2

    Rectangle {
        id: rec
        anchors.fill: parent
        radius: 50
        color: "transparent"
    }

    Text {
        id: icon
        font.family: "Material Design Icons"
        font.pixelSize: 96
        opacity: 0.5
        color: "black"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onHoveredChanged: {
            rec.color = containsMouse ? hoveredColor : "transparent";
            icon.opacity = containsMouse ? 1 : .5;
        }
    }
}
