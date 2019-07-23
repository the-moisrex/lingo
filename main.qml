import QtQuick 2.9
import QtQuick.Controls 2.5

ApplicationWindow {
    id: window
    visible: true
    width: 480
    height: 600
    property alias stackView: stackView
    title: qsTr("Lingo")


    header: ToolBar {
        id: toolBar
        contentHeight: inputId.implicitHeight + langSelectorId.height

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? Icons["arrowLeft"] : Icons["formatListBulleted"]
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            font.family: "Material Design Icons"
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        SearchInput {
            id: inputId
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.left: parent.left
            anchors.leftMargin: 50
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

        LanguageSelector {
            id: langSelectorId
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: inputId.bottom
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
            } else if (stackView.empty) {
                close.accepted = true;
                close.destroy()
            }
        }
    }

}





/*##^## Designer {
    D{i:3;anchors_width:0}
}
 ##^##*/
