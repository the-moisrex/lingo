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
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            height: dataParent.height

            Component.onCompleted: {
                console.log("aaa")
            }

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
                anchors.right: parent.right
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
                    text: title
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


            Switch {
                id: switchBtn
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10
                visible: type == 3 // resource_options::input_t::CHECKBOX
//                onCheckedChanged: {
//                    if (value !== checked)
//                        value = checked
//                }
                checked: value
                Binding {
                    target: model
                    property: "value"
                    value: switchBtn.checked
                }

//                Component.onCompleted:  {
//                    if (type == 3) {
//                        checked = value;
//                        value = Qt.binding(function() {
//                            return checked;
//                        });
//                        console.log(checked, value)
//                    }
//                }
            }

//            TextEdit {
//                anchors.top: dataParent.bottom
//                anchors.left: dataParent.left
//                anchors.right: dataParent.right
//                visible: type == 1 // resource_options::input_t::TEXT
//                onTextChanged: {
//                    value = text;
//                }
//                Component.onCompleted: {
//                    text = value;
//                }
//            }


        }


    }
}
