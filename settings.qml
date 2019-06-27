import QtQuick 2.0
import QtQuick.Controls 2.12

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
                id: hist_header
                text: qsTr("History:")
            }

            Switch {
                text: "Remember search histories"
                anchors.right: parent.right
                anchors.left: parent.left
                checked: MySettings.rememberHistory
                onCheckedChanged: {
                    MySettings.rememberHistory = checked;
                }
            }

            Button {
                text: "Clear search histories (" + MySettings.historyCount + ")"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    MySettings.clearHistory();
                }
            }

            Text {
                id: dic_header
                text: qsTr("Dictionaries:")
            }


        }
    }
}



/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
