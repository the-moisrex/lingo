import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Item {
    id: rootId
    property color primaryColor: Qt.darker(Material.primary, 1.3)
    property color lighterPrimaryColor: Qt.lighter(primaryColor, 1.5)
    property color darkerPriaryColor: Qt.darker(primaryColor, 1.5)
    signal textChanged(string value)
    height: inputId.implicitHeight + 13
    implicitHeight: height

    function clear() {
        inputId.text = "";
    }


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

        radius: 4
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
            color: Qt.lighter(primaryColor, 3)
            placeholderText: qsTr("search...")
            background: null
            anchors.right: parent.right;
            anchors.left: parent.left
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            selectByKeyboard: true
            selectByMouse: true
            placeholderTextColor: Qt.lighter(primaryColor, 2)
            selectionColor: Qt.lighter(primaryColor, 1.8)
            font.pixelSize: Qt.application.font.pixelSize * 1.2
            onTextChanged: rootId.textChanged(inputId.text)
        }


    }
}










































/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
