#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>
#include <QOnlineTranslator>
#include <future>
#include <mutex>

class Translator : public QObject {
 private:
  QOnlineTranslator::Language from = QOnlineTranslator::Language::Auto;
  QOnlineTranslator::Language to = QOnlineTranslator::Language::English;
  std::mutex m;
  QString _source, _translation, _translationTranslit,
      _translationLanguageString;
  bool loading = false;
  std::promise<void> stopSignal;
  std::future<void> stopFuture;

  void search(QString const& data);

 public:
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  Q_PROPERTY(QString source READ source NOTIFY sourceChanged)
  Q_PROPERTY(QString translation READ translation NOTIFY translationChanged)
  Q_PROPERTY(QString translationTranslit READ translationTranslit NOTIFY
                 translationTranslitChanged)
  Q_PROPERTY(QString translationLanguageString READ translationLanguageString
                 NOTIFY translationLanguageStringChanged)

 signals:
  void loadingChanged(bool loading);
  void translationChanged(QString translation);
  void sourceChanged(QString source);
  void translationTranslitChanged(QString translationTranslit);
  void translationLanguageStringChanged(QString translationLanguageString);

 public slots:
  void onTextChanged(QString value);

 public:
  explicit Translator(QObject* parent = nullptr);
  bool isLoading() const noexcept { return loading; }
  QString translation() const noexcept { return _translation; }
  QString source() const noexcept { return _source; }
  QString translationTranslit() const noexcept { return _translationTranslit; }
  QString translationLanguageString() const noexcept {
    return _translationLanguageString;
  }
};

#endif  // TRANSLATORMODEL_H
