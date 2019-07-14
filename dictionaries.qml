import QtQuick 2.9
import QtQuick.Controls 2.5

Page {
    id: root
    title: qsTr("Dictionaries")

    RoundButton {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 10
        anchors.bottomMargin: 10
        width: implicitHeight
        text: "+"
        font.pointSize: 20
        highlighted: true
        onClicked: {
            stackView.push(
                "create_dictionary.qml"
            )
        }
        z: 10
    }

    ListView {
        id: listView
        spacing: 10
        z: 5
        anchors.fill: parent
        delegate: Item {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            height: dataParent.height

            Rectangle {
                property color bgColor: "#eee"
                id: bg
                anchors.fill: parent
                anchors.topMargin: 3
                color: bgColor
                radius: 4
            }

            Item {
                id: dataParent
                anchors.right: settingsIcon.left
                anchors.rightMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
                height: data.implicitHeight * 3

                Label {
                    id: data
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.right: parent.right
                    font.pointSize: 17
                    text:  name + " <small>(" + (translatorEnabled ? ("<font color=green>" + qsTr("Enabled") + "</font>") : ("<font color=red>" + qsTr("Disabled") + "</font>")) + ")</small>"
                    textFormat: Qt.RichText
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    bg.bgColor = containsMouse ? "#e0e0e0" : "#eee"
                }
            }


            IconLabel {
                id: settingsIcon
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                text: Icons["settings"]
                font.pixelSize: 24
                hoverEnabled: true


                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        stackView.push(
                            "dictionary_settings.qml",
                            {
                                dictionary_index: index
                            }
                        )
                    }
                }
            }

        }
        model: Dictionaries
    }


}
