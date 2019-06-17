#include "translator.h"
#include <QDebug>
#include <QOnlineTranslator>

Translator::Translator(QObject* parent) : QObject(parent), translator(this) {}

void Translator::search(const QString& data) {
  // loading changes so the ui gets affected
  loading = true;
  emit loadingChanged(loading);

  translator.translate(data, QOnlineTranslator::Google);
  emit sourceChanged(data);

  if (translator.error() == QOnlineTranslator::NoError) {
    emit translationChanged(translation());
    emit translationTranslitChanged(translationTranslit());
    emit translationLanguageStringChanged(translationLanguageString());
  } else {
    qCritical() << translator.errorString();
  }

  loading = false;
  emit loadingChanged(loading);
}

void Translator::onTextChanged(QString value) {
  search(value);
}
