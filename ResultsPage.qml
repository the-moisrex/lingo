import QtQuick 2.0
import QtQuick.Controls 2.5

Item {
  id: rootId
  signal textChanged(string value)

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
      orientation: ListView.Horizontal
      keyNavigationWraps: true
      anchors.fill: parent
      //      visible: !Translator.loading
      visible: true
      model: Translator.model
      highlight: Rectangle {
          color: "lightsteelblue"
          radius: 5
      }
      delegate: Text {
          text: "fdsafds"
      }
  }


}





/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
