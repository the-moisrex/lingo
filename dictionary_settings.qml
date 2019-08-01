import QtQuick 2.9
import QtQuick.Controls 2.5
import Resource 1.0

Page {
    id: root
    title: qsTr("Dictionary settings")
    property int dictionary_index: 0

    Text {
        id: desc
        text: "<font color=gray>" + qsTr("Key: ") + "</font>" + optionsList.model.key() + "<br><font color=gray>" + qsTr("Name: ") + "</font> "+ optionsList.model.name() + "<br><font color=gray>" + qsTr("Description: ") + "</font>" + optionsList.model.description()
        wrapMode: Text.WordWrap
        textFormat: Qt.RichText
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 10
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        lineHeight: 1.5
    }

    IconLabel {
        font.pointSize: 24
        text: Icons["delete"]
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.rightMargin: 10
        highlighted: true
        z: 10
        visible: optionsList.model.isDeletable()
        onClicked: {
            Dictionaries.remove(optionsList.model.id());
            stackView.pop();
        }
    }

    ListView {
        id: optionsList
        model: Dictionaries.optionsModel(dictionary_index)
        spacing: 10
        z: 5
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: desc.bottom
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
        onModelChanged: {
            model.componentComplete();
            update()
        }

        delegate: Item {
            id: childMaster
            anchors.right: parent.right
            anchors.left: parent.left

            Component.onCompleted: {
                let item = null;
                switch (type) {
                case 1:
                    item = a_text;
                    break;
                case 3:
                    item = a_switch;
                    break;
                }
                if (!item)
                    return;
                if (item.status === Component.Ready) {
                   let ref = item.createObject(childMaster, {
                                                title, model, type, value
                                            });
                    childMaster.height = Qt.binding(() => {
                                                        return ref.height;
                                                    });
                }
            }

        }

    }


    /////////////////////////////////////////////////////////////////////////

    Component {
        id: a_switch

        Item {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            height: data.implicitHeight * 3

            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0

            Rectangle {
                property color bgColor: "#eee"
                id: bg
                anchors.fill: parent
                anchors.topMargin: 3
                color: bgColor
                radius: 4
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onHoveredChanged: {
                    bg.bgColor = containsMouse ? "#e0e0e0" : "#eee"
                }
                z: 100
            }


            Label {
                id: data
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                font.pointSize: 16
                text: title
                textFormat: Qt.RichText
                z: 90
            }

            Switch {
                id: switchBtn
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                z: 200
                checked: value
                Binding {
                    target: model
                    property: "value"
                    value: switchBtn.checked
                }

            }

        }

    }





    /////////////////////////////////////////////////////////////////


    Component {
        id: a_text

        Item {
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            height: data.implicitHeight * 3

            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0


            Rectangle {
                property color bgColor: "#eee"
                id: bg
                anchors.fill: parent
                anchors.topMargin: 3
                color: bgColor
                radius: 4
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                z: 100
                onHoveredChanged: {
                    bg.bgColor = containsMouse ? "#e0e0e0" : "#eee"
                }
            }


            Label {
                id: data
                anchors.verticalCenter: parent.verticalCenter
                anchors.verticalCenterOffset: -15
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                font.pointSize: 16
                text: title
                textFormat: Qt.RichText
                z: 90
            }

            TextField {
                id: txtInpt
                anchors.top: data.bottom
                anchors.left: data.left
                anchors.right: data.right
                text: value
                selectByMouse: true
                z: 200
                Binding {
                    target: model
                    property: "value"
                    value: txtInpt.text
                }
            }



        }
    }

}
