#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QOnlineTranslator>
#include "resource.h"

QString formatData(const QOnlineTranslator& translator);

template <QOnlineTranslator::Engine Engine>
class OnlineTranslator : public Resource {
 private:
  QOnlineTranslator* translator;

 public:
  explicit OnlineTranslator(QObject* parent = nullptr)
      : Resource(parent), translator(new QOnlineTranslator(this)) {
    QObject::connect(translator, &QOnlineTranslator::finished, [&] {
      if (translator->error() == QOnlineTranslator::NoError) {
        auto t = formatData(*translator);
        //        qDebug() << t;
        setTranslation(t);
      } else {
        clearTranslation();
        qCritical() << Engine << translator->errorString();
      }

      setLoading(false);
    });
  }

  void search(QString const& data) override {
    // loading changes so the ui gets affected
    setLoading(true);
    qDebug() << QOnlineTranslator::languageCode(getFromLang())
             << QOnlineTranslator::languageCode(getToLang());
    translator->translate(data, Engine, getToLang(), getFromLang());
  }

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

  void componentComplete() override {
    Resource::componentComplete();
    setInitStatus(false);
  }
};

// explicit template instanciations
// template class OnlineTranslator<QOnlineTranslator::Google>;
// template class OnlineTranslator<QOnlineTranslator::Bing>;
// template class OnlineTranslator<QOnlineTranslator::Yandex>;

// registering it for the qml
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Google>);
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Bing>);
// QML_DECLARE_TYPE(OnlineTranslator<QOnlineTranslator::Yandex>);

#endif  // TRANSLATORMODEL_H
