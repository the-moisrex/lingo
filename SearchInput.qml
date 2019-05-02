import QtQuick 2.0
import QtQuick.Controls 2.3
Item {
    id: rootId
    property color primaryColor: "black"
    property color secondaryColor: "blue"
    property color lighterPrimaryColor: Qt.lighter(primaryColor, 1.5)
    property color darkerPriaryColor: Qt.darker(primaryColor, 1.5)
    signal onTextChanged(string text)


    Rectangle {
//        color: inputId.activeFocus ? primaryColor : Qt.lighter(primaryColor, 1.2)
        gradient: Gradient {
            GradientStop {
                position: 0.0
                SequentialAnimation on color {
                    loops: Animation.Infinite
                    ColorAnimation { from: lighterPrimaryColor; to: darkerPriaryColor; duration: 5000 }
                    ColorAnimation { from: darkerPriaryColor; to: lighterPrimaryColor; duration: 5000 }
                }
            }
            GradientStop { position: 1.0; color: primaryColor }
        }

        radius: 50
        anchors.bottomMargin: 3
        anchors.topMargin: 3
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        border {
            color: inputId.activeFocus ? Qt.darker(primaryColor, 1.3) : primaryColor
            width: 2
        }

        TextArea {
            id: inputId
            bottomPadding: 0
            placeholderText: qsTr("search...")
            background: null
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.fill: parent
            selectByKeyboard: true
            selectByMouse: true
            selectionColor: secondaryColor
            font.pixelSize: Qt.application.font.pixelSize * 1.2
            onTextChanged: rootId.onTextChanged(text)
        }


    }
}










































/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
