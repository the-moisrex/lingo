#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include "resource.h"
#include <QAbstractListModel>
#include <QDebug>
#include <QOnlineTranslator>

QString formatData(const QOnlineTranslator &translator);

class OnlineTranslator : public Resource {
  Q_OBJECT

  QOnlineTranslator *translator;
  QOnlineTranslator::Engine engine = QOnlineTranslator::Google;

public:
  explicit OnlineTranslator(QObject *parent);

  ~OnlineTranslator() override {}

  void search(QString const &data) override;

  QString key() const noexcept override;

  QString name() const noexcept override {
    switch (engine) {
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
                   QOnlineTranslator::Language to) const noexcept override;

  bool canProvideSuggestions() const noexcept override { return false; }

  void componentComplete() override;
  QOnlineTranslator::Engine getEngine() const;
  void setEngine(const QOnlineTranslator::Engine &value);
};

// explicit template instanciations
// template class OnlineTranslator<QOnlineTranslator::Google>;
// template class OnlineTranslator<QOnlineTranslator::Bing>;
// template class OnlineTranslator<QOnlineTranslator::Yandex>;

// registering it for the qml
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Google>);
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Bing>);
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Yandex>);

#endif // TRANSLATORMODEL_H
