import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Item {
  id: rootId
  signal textChanged(string value)
  property string lastValue: ""

  Timer {
      id: timer
      repeat: false
      interval: 500
      onTriggered: Dictionaries.search(lastValue)
  }

  Component.onCompleted: {
      rootId.textChanged.connect(function(value) {
          lastValue = value;
          timer.stop();
          timer.start(); // debouncing
      });
  }


  ListView {
      id: dictList
      anchors.fill: parent
      model: Dictionaries
      delegate: Item {
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.leftMargin: 10
          anchors.rightMargin: 10

          property bool isThisEnabled: translatorEnabled && tempEnabled && !hidden

          height: isThisEnabled ? cols.implicitHeight : 0
          visible: isThisEnabled

          Item {
              anchors.fill: parent
              anchors.topMargin: isThisEnabled ? 10 : 0

              BusyIndicator {
                  id: loadingId
                  width: 50
                  height: 50
                  anchors.centerIn: parent
                  visible: loading
              }

              Column {
                  id: cols
                  anchors.fill: parent
                  anchors.topMargin: 10


                  Text {
                      text: name
                      font.pointSize: translationId.font.pointSize * .7
                      color: Material.accent
                  }

                  Text {
                      id: translationId
                      text: translation == "" ? qsTr("No result") : translation
                      textFormat: Text.RichText
                      wrapMode: Text.Wrap
                      anchors.left: parent.left
                      anchors.right: parent.right
                  }
              }

              IconLabel {
                  anchors.right: parent.right
                  anchors.top: parent.top
                  text: Icons["voice"]
                  font.pixelSize: 24
                  hoverEnabled: true
                  visible: translation != ""

                  MouseArea {
                      anchors.fill: parent
                      onClicked: {
                          Speech.say(Dictionaries.readableTranslation(translation));
                      }
                  }
              }
          }
      }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
