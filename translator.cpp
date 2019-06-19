#include "translator.h"
#include <QDebug>
#include <QOnlineTranslator>
#include <future>
#include <mutex>

Translator::Translator(QObject* parent) : QObject(parent) {}

void Translator::search(const QString& data) {
  std::lock_guard lock(m);
  // loading changes so the ui gets affected
  loading = true;
  emit loadingChanged(loading);
  static thread_local QOnlineTranslator translator;

  translator.translate(data, QOnlineTranslator::Google);

  if (translator.error() == QOnlineTranslator::NoError) {
    _translation = translator.translation();
    _source = translator.source();
    _translationTranslit = translator.translationTranslit();
    _translationLanguageString = translator.translationLanguageString();
    emit sourceChanged(data);
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
  auto a = std::async(&Translator::search, this, value);
}
