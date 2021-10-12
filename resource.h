#ifndef RESOURCE_H
#define RESOURCE_H

#include "settings.h"
#include <QAbstractListModel>
#include <QObject>
#include <QOnlineTranslator>
#include <QQmlEngine>
#include <QReadLocker>
#include <QReadWriteLock>
#include <QString>
#include <QWriteLocker>
#include <utility>

#include "resource_options_model.h"

inline uint qHash(const resource_option &key, uint seed) {
  return qHash(key.input_type, seed) ^ qHash(key.key, seed) ^
         qHash(key.value.toString(), seed) ^ qHash(key.title, seed) ^
         qHash(key.choices, seed) ^ [&]() {
           auto hashed = seed;
           for (auto const &item : key.available_options) {
             for (auto const &item2 : item) {
               hashed ^= qHash(item2, seed);
             }
           }
           return hashed;
         }.operator()();
}

class OnlineTranslator;

class Resource : public QAbstractListModel, public QQmlParserStatus {
  Q_OBJECT
  Q_INTERFACES(QQmlParserStatus)

  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  Q_PROPERTY(bool initStatus READ isInitializing NOTIFY initStatusChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
  Q_PROPERTY(bool tempEnabled READ isTempEnabled WRITE setTempEnabled NOTIFY
                 tempEnabledChanged)
  Q_PROPERTY(QString translation READ translation NOTIFY translationChanged)
  Q_PROPERTY(bool hidden READ isHidden WRITE hide NOTIFY hideChanged)

protected:
  mutable QReadWriteLock loadingLock, translationLock, enabledLock,
      initStatusLock;
  mutable QVector<resource_option> options_cache;
  mutable QSet<resource_option> default_options;
  QString _translation;
  QString _id;
  QString _name;
  bool loading = false;
  bool initilizing = true;
  bool tempEnabled = true;
  bool deletable = true;
  bool ishidden = false;

  void setLoading(bool newLoadingValue) {
    {
      QWriteLocker locked(&loadingLock);
      loading = newLoadingValue;
    }
    emit loadingChanged(this, loading);
  }

  void setInitStatus(bool newInitializingValue) {
    {
      QWriteLocker locker(&initStatusLock);
      initilizing = newInitializingValue;
    }
    emit initStatusChanged(this, initilizing);
  }

  void setTranslation(QString newTranslation) {
    {
      QWriteLocker locker(&translationLock);
      _translation = newTranslation;
    }
    emit translationChanged(this, _translation);
  }

signals:
  void loadingChanged(Resource *self, bool loading);
  void initStatusChanged(Resource *self, bool initializationStatus);
  void enabledChanged(Resource *self, bool enabled);
  void tempEnabledChanged(Resource *self, bool tmpEnabled);
  void translationChanged(Resource *self, QString translation);
  void hideChanged(Resource *self, bool ishidden);

public:
  ~Resource() override = default;

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  // Editable:
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;

  Q_INVOKABLE void componentComplete() override;
  Q_INVOKABLE void classBegin() override;
  Q_INVOKABLE ResourceOptionsModel *availableOptionsModel(int index,
                                                          QString const &key);

public:
  explicit Resource(QObject *parent = nullptr);
  virtual void search(QString const & /* data */) {
    // do nothing
  }
  Q_INVOKABLE virtual QString translation() const noexcept {
    QReadLocker locker(&translationLock);
    return _translation;
  }

  Q_INVOKABLE virtual bool isHidden() const noexcept { return ishidden; }

  Q_INVOKABLE virtual void hide(bool val) noexcept {
    ishidden = val;
    emit hideChanged(this, val);
  }

  Q_INVOKABLE virtual bool isDeletable() const noexcept { return deletable; }

  Q_INVOKABLE virtual void setDeletable(bool value) noexcept {
    deletable = value;
  }

  /**
   * @brief name of the resource in English or other languages
   * @return QString
   */
  Q_INVOKABLE virtual QString name() const noexcept {
    return _name == "" ? tr("UnNamed Translator") : _name;
  }

  /**
   * @brief set name
   */
  Q_INVOKABLE virtual void setName(QString newName) noexcept {
    _name = std::move(newName);
  }

  /**
   * @brief key to use in options
   * @return
   */
  Q_INVOKABLE virtual QString key() const noexcept = 0;

  /**
   * @brief Set Id
   */
  Q_INVOKABLE virtual void setId(QString newId) noexcept {
    _id = std::move(newId);
  }

  /**
   * @brief get id
   */
  Q_INVOKABLE virtual QString id() const noexcept {
    return _id == "" ? key() : _id;
  }

  /**
   * @brief description of the resouce
   * @return QString
   */
  Q_INVOKABLE virtual QString description() const noexcept { return ""; }

  /**
   * @brief checks if the specified languages supported for this resource or not
   * @param from a language
   * @param to a language
   * @return bool
   */
  Q_INVOKABLE virtual bool
  isSupported(QOnlineTranslator::Language /* from */,
              QOnlineTranslator::Language /* to */) const noexcept {
    return false; // default value
  }

  Q_INVOKABLE virtual void clearTranslation() noexcept { setTranslation(""); }

  Q_INVOKABLE virtual bool isTempEnabled() const noexcept {
    return tempEnabled;
  }

  Q_INVOKABLE virtual void setTempEnabled(bool value) noexcept {
    tempEnabled = value;
    emit tempEnabledChanged(this, value);
  }

  /**
   * @brief check if the resource is enabled or not
   * @return
   */
  Q_INVOKABLE virtual bool isEnabled() const noexcept {
    QReadLocker locker(&enabledLock);
    return optionValue("enabled", true).toBool();
  }

  /**
   * @brief changes the enabled option
   * @return
   */
  Q_INVOKABLE virtual void setEnabled(bool enabled);

  /**
   * @brief can provide suggestions
   * @return
   */
  Q_INVOKABLE virtual bool canProvideSuggestions() const noexcept {
    return false;
  }

  /**
   * @brief returns a list of suggestion words
   * @return
   */
  Q_INVOKABLE virtual QVector<QString> suggestions() const noexcept {
    return {}; // empty result
  }

  /**
   * @brief is loading the resource
   * @return
   */
  Q_INVOKABLE virtual bool isLoading() const noexcept {
    QReadLocker locker(&loadingLock);
    return loading;
  }

  /**
   * @brief is the resource initializing (for example: connecting to the MySQL
   * database). this funciton will be used in the splash screen to make sure the
   * resource is connected to the database
   * @return
   */
  Q_INVOKABLE virtual bool isInitializing() const noexcept {
    QReadLocker locker(&initStatusLock);
    return initilizing;
  }

  /**
   * @brief get an option
   * @param _key
   * @param defaultValue
   * @return
   */
  virtual const resource_option &option(QString const &_key) const;

  virtual bool optionExists(QString const &_key) const;

  /**
   * @brief returns a value of an option if exists
   * @param _key
   * @return
   */
  virtual QVariant optionValue(QString const &_key,
                               const QVariant &defaultValue = QVariant()) const;

  /**
   * @brief get a reference to all the options (options are cached in memory)
   * @return
   */
  virtual const QVector<resource_option> &options() const;

  /**
   * @brief set an option
   * @param _key
   * @param value
   */
  virtual void setOption(resource_option const &the_option);

  /**
   * @brief setOptionIfNotExists
   * @param the_option
   */
  virtual void setOptionIfNotExists(resource_option const &the_option);

  /**
   * @brief reloadOptionsCache
   */
  virtual void reloadOptionsCache() const;

  virtual QOnlineTranslator::Language getFromLang();

  virtual QOnlineTranslator::Language getToLang();

  //  OnlineTranslator<QOnlineTranslator::Google> *toOnlineGoogle(Resource *res)
  //  {
  //    return reinterpret_cast<OnlineTranslator<QOnlineTranslator::Google>
  //    *>(res);
  //  }
  //  OnlineTranslator<QOnlineTranslator::Bing> *toOnlineBing(Resource *res) {
  //    return reinterpret_cast<OnlineTranslator<QOnlineTranslator::Bing>
  //    *>(res);
  //  }
  //  OnlineTranslator<QOnlineTranslator::Yandex> *toOnlineYandex(Resource *res)
  //  {
  //    return reinterpret_cast<OnlineTranslator<QOnlineTranslator::Yandex>
  //    *>(res);
  //  }
};

// registering it for the qml
// QML_DECLARE_TYPE(Resource)

#endif // RESOURCE_H
