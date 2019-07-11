import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Page {
    title: qsTr("Create new resource")


    Flickable {
        anchors.fill: parent

        Column {
            spacing: 20
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.topMargin: 10
            anchors.rightMargin: 10
            anchors.bottomMargin: 10

            TextField {
                id: dicName
                selectByMouse: true
                anchors.right: parent.right
                anchors.left: parent.left
                placeholderText: qsTr("Name")
                placeholderTextColor: Material.accent
            }

            ComboBox {
                id: proto
                model: Dictionaries.prototypes
                anchors.right: parent.right
                anchors.left: parent.left
                textRole: "name"
            }

            Button {
                text: qsTr("Save")
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    Dictionaries.create(dicName.text, proto.currentIndex);
                    stackView.pop()
                }
            }

        }
    }

}
