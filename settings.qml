import QtQuick 2.0
import QtQuick.Controls 2.12

Page {
    id: root
    title: qsTr("Settings")

    Flickable {
       id: settingsList

       Row {
           id: row
           anchors.fill: parent
           Text {
               text: qsTr("History:")
               anchors.right: parent.right
               anchors.left: parent.left
         }

       }
    }
}
