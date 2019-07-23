import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Controls.Material 2.3

Item {
    id: root
    height: Math.max(fromLang.implicitHeight, toLang.implicitHeight)
    implicitHeight: height


    Rectangle {
        id: rectangle
        color: Qt.lighter(Material.background, 1.3)
        anchors.left: parent.left
        anchors.right: parent.right

        Row {
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: spacing
            spacing: 5


            ComboBox {
                id: fromLang
                property bool changed: false
                model: Dictionaries.fromLangsModel()
                width: (root.width / 2 - switchBtn.implicitWidth / 2) - (parent.spacing * (parent.children.length + 1) / 2)
                currentIndex: Dictionaries.getFromLang()
                onCurrentIndexChanged: {
                    if (changed)
                        Dictionaries.setFromLang(currentIndex);
                    else
                        changed = true;
                }
                onModelChanged: currentIndex = Dictionaries.getFromLang()
            }

            RoundButton {
                id: switchBtn
                text: "⇌"
                flat: true
                onClicked: {
                    let from = fromLang.currentIndex;
                    let to = toLang.currentIndex;
                    fromLang.currentIndex = to + 1;
                    toLang.currentIndex = from === 0 ? 22 : (from - 1); // Default: English
                }
            }

            ComboBox {
                id: toLang
                property bool changed: false
                model: Dictionaries.toLangsModel()
                width: (root.width / 2 - switchBtn.implicitWidth / 2) - (parent.spacing * (parent.children.length + 1) / 2)
                onCurrentIndexChanged: {
                    if (changed)
                        Dictionaries.setToLang(currentIndex);
                    else
                        changed = true;
                }
                onModelChanged: currentIndex = Dictionaries.getToLang()
            }

        }

    }

}
