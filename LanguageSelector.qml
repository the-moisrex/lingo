import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Item {
    id: root
    height: Math.max(fromLang.implicitHeight, toLang.implicitHeight)


    Rectangle {
        id: rectangle
        color: Qt.lighter(Material.background, 1.3)
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            anchors.left: parent.left
            anchors.right: parent.right


            ComboBox {
                id: fromLang
                model: Dictionaries.fromLangsModel()
                width: root.implicitWidth / 2 - 10
            }

            ComboBox {
                id: toLang
                model: Dictionaries.toLangsModel()
                width: root.implicitWidth / 2 - 10
            }

        }

    }

}
