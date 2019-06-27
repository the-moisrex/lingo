import QtQuick 2.0
import QtQuick.Controls 2.12

Page {
    id: root
    title: qsTr("Dictionaries")

    ListView {
        id: listView
        spacing: 20
        anchors.fill: parent
        delegate: Item {
            Text {
                text: name + "(<font color=green>" + (enabled ? qsTr("Enabled") : qsTr("Disabled")) + "</font>)"
            }
        }
        model: Dictionaries
    }


}
