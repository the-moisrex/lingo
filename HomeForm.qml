import QtQuick 2.9
import QtQuick.Controls 2.5

Page {
    id: page
    title: qsTr("Home")

    Label {
        id: info
        text: qsTr("Search something...")
        font.pointSize: 18
        anchors.centerIn: parent
    }

    ProgressBar {
        id: initLoading
        anchors.top: info.bottom
        anchors.topMargin: 10;
        anchors.left: info.left
        anchors.right: info.right
        value: Dictionaries.initStatusPercent
        onValueChanged: {
            if (value >= .98)
                visible = false;
            else
                visible = true;
        }

        Component.onCompleted: Dictionaries.init()
    }
}
