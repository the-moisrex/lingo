import QtQuick 2.0
import QtQuick.Controls 2.3
Item {
    id: rootId
    property color primaryColor: "black"


    Rectangle {
        color: Qt.lighter(primaryColor, 1.3)
        radius: 50
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        border {
            color: primaryColor
            width: 1
        }

        TextField {
            id: inputId
            text: "what"
            anchors.fill: parent
            font.pixelSize: Qt.application.font.pixelSize * 1.2
        }
    }
}


/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
