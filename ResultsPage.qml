import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import Translator 1.0

Item {
  id: rootId
  signal textChanged(string value)

  property color secondaryColor: "white"

  Component.onCompleted: {
      rootId.textChanged.connect(function(value) {
          Translator.onTextChanged(value);
      });
  }

  BusyIndicator {
      id: loadingId
      width: 50
      height: 50
      anchors.centerIn: parent
      visible: Translator.loading
  }


  ListView {
      id: resultsId
      orientation: ListView.Vertical
      keyNavigationWraps: true
      clip: true
      anchors.fill: parent
      //      visible: !Translator.loading
      visible: true
      model: TranslatorModel {}
//      model: 100
      highlight: Rectangle {
          color: "#666"
          radius: 5
      }
      delegate: RowLayout {
          width: parent.width
          Text {
              Layout.fillWidth: true
              Layout.margins: 10
              text: model.word
              color: secondaryColor
          }
      }
  }


}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
