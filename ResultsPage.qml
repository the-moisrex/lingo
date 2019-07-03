import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3
import "icon.js" as MIcons

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
      spacing: 10
      delegate: Item {
          anchors.left: parent.left
          anchors.right: parent.right
          anchors.leftMargin: 10
          anchors.rightMargin: 10
          height: cols.implicitHeight

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
              text: MIcons.Icon.voice
              font.pixelSize: 24
              hoverEnabled: true
              visible: translation != ""

              MouseArea {
                  anchors.fill: parent
                  onClicked: {
                      console.log(translation);
                      Speech.say(translation);
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
