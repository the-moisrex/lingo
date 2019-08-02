import QtQuick 2.9
import QtQuick.Controls 2.5
import Resource 1.0

Page {
    id: root
    title: qsTr("Dictionary settings")
    property int dictionary_index: 0

//    Text {
//        id: desc
//        text: "<font color=gray>" + qsTr("Key: ") + "</font>" + optionsList.model.key() + "<br><font color=gray>" + qsTr("Name: ") + "</font> "+ optionsList.model.name() + "<br><font color=gray>" + qsTr("Description: ") + "</font>" + optionsList.model.description()
//        wrapMode: Text.WordWrap
//        textFormat: Qt.RichText
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.topMargin: 10
//        anchors.leftMargin: 10
//        anchors.rightMargin: 10
//        lineHeight: 1.5
//    }

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
        anchors.fill: parent
        onModelChanged: {
            model.componentComplete();
            update()
        }

        delegate: Item {
            id: childMaster
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.leftMargin: 5
            anchors.rightMargin: 5

            Rectangle {
                property color bgColor: "#eee"
                id: bg
                anchors.fill: parent
                anchors.topMargin: 3
                color: bgColor
                radius: 4
            }

            Label {
                id: titleLabel
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 10
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                font.pointSize: 15
                text: title ? title : ""
                textFormat: Qt.RichText
            }



            Component.onCompleted: {
                let item = null;
                switch (type) {
                case 1:
                    item = a_text;
                    break;
                case 2:
                    item = a_long_text;
                    break;
                case 3:
                    item = a_switch;
                    break;
                case 4:
                    item = a_multichoice;
                    break;
                case 5:
                    item = a_options_switcher;
                    break;
                default:
                    item = a_simple_text;
                    titleLabel.visible = false;
                    titleLabel.height = 0;
                }
                if (!item)
                    return;
                if (item.status === Component.Ready) {
                   let ref = item.createObject(childMaster, {
                                                title, model, type, value, choices
                                            });
                    switch (type) {
                    case 3: // checkbox
                        childMaster.implicitHeight = Qt.binding(() => {
                            return Math.max(titleLabel.implicitHeight, ref.implicitHeight, ref.height) + 10;
                        });
                        titleLabel.anchors.verticalCenter = Qt.binding(() => {
                            return childMaster.verticalCenter;
                        });
                        break;
                    default:
                        ref.anchors.top = titleLabel.bottom;
                        titleLabel.anchors.top = Qt.binding(() => childMaster.top);
                        childMaster.height = Qt.binding(() => {
                            return Math.max(ref.implicitHeight, ref.height)  + titleLabel.implicitHeight + 10;
                        });
                    }
                }
            }

        }

    }


    /////////////////////////////////////////////////////////////////////////

    Component {
        id: a_switch

        Switch{
            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0

            id: switchBtn
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10
            checked: value
            Binding {
                target: model
                property: "value"
                value: switchBtn.checked
            }

        }

    }





    /////////////////////////////////////////////////////////////////


    Component {
        id: a_text

        TextField {
            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0

            id: txtInpt
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            text: value
            selectByMouse: true
            Binding {
                target: model
                property: "value"
                value: txtInpt.text
            }



        }
    }



    ///////////////////////////////////////////////////////////////////

    Component {
        id: a_long_text

        TextArea {
            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0

            id: txtInpt
            anchors.left: parent.left
            anchors.right: parent.right
            text: value
            selectByMouse: true
            Binding {
                target: model
                property: "value"
                value: txtInpt.text
            }



        }
    }

    ///////////////////////////////////////////////////////////////////

    Component {
        id: a_simple_text

        Text {
            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0

            id: desc
            text: value
            wrapMode: Text.WordWrap
            textFormat: Qt.RichText
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: 10
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            lineHeight: 1.2
      }

    }


    //////////////////////////////////////////////////////////////////

    Component {
        id: a_multichoice

        ComboBox {
            id: comboId

            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0
            property var choices: 0

            model: choices
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            currentIndex: value
            Binding {
                target: model
                property: "value"
                value: comboId.currentIndex
            }
        }
    }



    ///////////////////////////////////////////////////////////////////


    Component {
        id: a_options_switcher

        ComboBox {
            id: comboOptionsId

            property var model: 0
            property var title: 0
            property var value: 0
            property var type: 0
            property var choices: 0

            model: choices
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            currentIndex: value
            Binding {
                target: model
                property: "value"
                value: comboOptionsId.currentIndex
            }
        }
    }
}
