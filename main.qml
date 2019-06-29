import QtQuick 2.9
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    visible: true
    width: 480
    height: 800
    property alias stackView: stackView
    title: qsTr("Stack")

    property color primaryColor: "#333"
    property color secondaryColor: "#06a"

    header: ToolBar {
        id: toolBar
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        SearchInput {
            primaryColor: window.primaryColor
            secondaryColor: window.secondaryColor
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            onTextChanged: {
                var itemData = {objectName: "results-page"};
                if (value.trim() === "")
                    stackView.pop();
                else if (stackView.currentItem.objectName !== itemData.objectName)
                    stackView.push("ResultsPage.qml", itemData)
                if (value && value.trim() != "")
                    stackView.currentItem.textChanged(value);
            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.5
        height: window.height

        Column {
            anchors.fill: parent
            anchors.topMargin: toolBar.height

            ItemDelegate {
                text: qsTr("Home")
                width: parent.width
                onClicked: {
                    stackView.clear();
                    stackView.push("HomeForm.ui.qml")
                    drawer.close()
                }
            }

            ItemDelegate {
                text: qsTr("Settings")
                width: parent.width
                onClicked: {
                    stackView.push("settings.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.ui.qml"
        anchors.fill: parent
    }


    onClosing: {
        if (Qt.platform.os == "android") {
            if (stackView.depth > 1) {
                close.accepted = false
                stackView.pop()
            }
        }
    }

}





/*##^## Designer {
    D{i:3;anchors_width:0}
}
 ##^##*/
