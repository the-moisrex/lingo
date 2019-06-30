import QtQuick 2.9
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.3

Item {
  id: rootId
  signal textChanged(string value)


  Component.onCompleted: {
      rootId.textChanged.connect(function(value) {
//         Dictionaries.search(value);
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
          visible: enabled

          BusyIndicator {
              id: loadingId
              width: 50
              height: 50
              anchors.centerIn: parent
              visible: loading
          }

          Text {
              text: name
              font.pointSize: translationId.font.pointSize * .7
              color: Qt.lighter(Material.foreground, 1.5)
          }

          Text {
              id: translationId
              text: translation
          }
      }
  }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
