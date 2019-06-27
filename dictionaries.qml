import QtQuick 2.0
import QtQuick.Controls 2.12
import Dictionaries 1.0

Page {
    id: root
    title: qsTr("Dictionaries")

    ListView {
        id: listView
        spacing: 10
        anchors.fill: parent
        delegate: Item {

        }
        model: DictionariesListModel {

        }
    }


}
