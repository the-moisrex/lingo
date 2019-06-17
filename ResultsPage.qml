import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

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

  Item {
      visible: !Translator.loading
      anchors.fill: parent

      ColumnLayout {
          id: rowLayout
          anchors.fill: parent

          Text {
              id: sourceId
              text: Translator.source
          }

          Text {
              id: translationId
              text: Translator.translation
          }
      }

  }



//  ListView {
//      id: resultsId
//      orientation: ListView.Vertical
//      keyNavigationWraps: true
//      clip: true
//      anchors.fill: parent
//      visible: !trModel.loading
//      model: TranslatorModel {
//        id: trModel
//      }
////      model: 100
//      highlight: Rectangle {
//          color: "#666"
//          radius: 5
//      }
//      delegate: RowLayout {
//          width: parent.width
//          Text {
//              Layout.fillWidth: true
//              Layout.margins: 10
//              text: model.translation
//              color: secondaryColor
//          }
//      }
//  }


}







/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
