#include "onlinetranslators.h"
#include <QDebug>
#include <QOnlineTranslator>

template <QOnlineTranslator::Engine Engine>
void OnlineTranslator<Engine>::search(const QString& data) {
  // loading changes so the ui gets affected
  loading = true;
  emit loadingChanged(loading);
  static QOnlineTranslator translator;

  translator.translate(data, QOnlineTranslator::Google);

  if (translator.error() == QOnlineTranslator::NoError) {
    _translation = translator.translation();
    _source = translator.source();
    _translationTranslit = translator.translationTranslit();
    _translationLanguageString = translator.translationLanguageString();
    emit translationChanged(translation());
  } else {
    qCritical() << translator.errorString();
  }

  loading = false;
  emit loadingChanged(loading);
}
