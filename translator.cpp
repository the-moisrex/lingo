#include "translator.h"

Translator::Translator(QObject* parent) : QObject(parent) {
  resultsModel = new QStringListModel(this);
}

void Translator::onTextChanged(QString value) {
  // loading changes so the ui gets affected
  loading = true;
  emit loadingChanged(loading);

  QStringList list;
  list << "Some Random Data" << value;
  resultsModel->setStringList(list);

  loading = false;
  emit loadingChanged(loading);
}
