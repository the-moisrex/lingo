import QtQuick 2.9
import QtQuick.Controls 2.5

Page {
    id: root
    title: qsTr("Settings")


    Flickable {
        id: settingsList
        anchors.fill: parent

        Column {
            id: row
            anchors.topMargin: 10
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            spacing: 10
            anchors.fill: parent

            Text {
                text: qsTr("UI Language:")
                anchors.right: parent.right
                anchors.left: parent.left
            }

            ComboBox {
                property bool inited: false
                model: MySettings.getUILanguages()
                currentIndex: MySettings.getUILanguage()
                onCurrentIndexChanged:  {
                    if (inited) {
                        MySettings.setUILanguage(currentIndex);
                        restartBtn.visible = true;
                    }
                }
                Component.onCompleted: inited = true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Button {
                id: restartBtn
                visible: false
                text: qsTr("You need to restart the app, click here.")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    MySettings.restart();
                }
            }

            Text {
                id: hist_header
                text: qsTr("History:")
                anchors.right: parent.right
                anchors.left: parent.left
            }

            Switch {
                text: qsTr("Remember search histories")
                anchors.right: parent.right
                anchors.left: parent.left
                checked: MySettings.rememberHistory
                onCheckedChanged: {
                    MySettings.rememberHistory = checked;
                }
            }

            Button {
                text: qsTr("Clear search histories (%1)").arg(MySettings.historyCount)
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    MySettings.clearHistory();
                }
            }

            Text {
                id: dic_header
                text: qsTr("Dictionaries:")
                anchors.right: parent.right
                anchors.left: parent.left
            }

            Button {
                text: qsTr("Dictionaries")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    stackView.push("dictionaries.qml")
                }
            }

        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
