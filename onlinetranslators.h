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
};

// explicit template instanciations
template class OnlineTranslator<QOnlineTranslator::Google>;
template class OnlineTranslator<QOnlineTranslator::Bing>;
template class OnlineTranslator<QOnlineTranslator::Yandex>;

#endif  // TRANSLATORMODEL_H
