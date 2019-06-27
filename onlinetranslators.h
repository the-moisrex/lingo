#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>
#include <QOnlineTranslator>
#include <future>
#include <mutex>
#include "resource.h"

template <QOnlineTranslator::Engine Engine>
class OnlineTranslator : public Resource {
 private:
  QOnlineTranslator::Language from = QOnlineTranslator::Language::Auto;
  QOnlineTranslator::Language to = QOnlineTranslator::Language::English;
  std::mutex m;
  QString _source, _translation, _translationTranslit,
      _translationLanguageString;
  std::promise<void> stopSignal;
  std::future<void> stopFuture;

 public:
  QString translation() const noexcept override { return _translation; }
  QString source() const noexcept { return _source; }
  void search(QString const& data) override;

  QString key() const noexcept override {
    switch (Engine) {
      case QOnlineTranslator::Google:
        return "google";
      case QOnlineTranslator::Bing:
        return "bing";
      case QOnlineTranslator::Yandex:
        return "yandex";
    }
    return "";
  }

  QString name() const noexcept override {
    switch (Engine) {
      case QOnlineTranslator::Google:
        return tr("Google Translator");
      case QOnlineTranslator::Bing:
        return tr("Bing Translator");
      case QOnlineTranslator::Yandex:
        return tr("Yandex Translator");
    }
    return tr("Unknown Translator");
  }

  QString description() const noexcept override {
    return tr("%1 is an online translator that requires internet connection. "
              "This translator sometimes may deny service for various reasons.")
        .arg(name());
  }

  bool isSupported(QOnlineTranslator::Language from,
                   QOnlineTranslator::Language to) const noexcept override {
    return QOnlineTranslator::isSupportTranslation(Engine, to) &&
           QOnlineTranslator::isSupportTranslation(Engine, from);
  }

  bool canProvideSuggestions() const noexcept override { return false; }
};

// explicit template instanciations
template class OnlineTranslator<QOnlineTranslator::Google>;
template class OnlineTranslator<QOnlineTranslator::Bing>;
template class OnlineTranslator<QOnlineTranslator::Yandex>;

#endif  // TRANSLATORMODEL_H
