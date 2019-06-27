#ifndef RESOURCE_H
#define RESOURCE_H

#include <QAbstractListModel>
#include <QObject>
#include <QOnlineTranslator>
#include <QString>
#include <utility>
#include "settings.h"

struct resource_option {
  enum input_t { TEXT = 1, TEXT_LONG, CHECKBOX };
  input_t input_type;
  QString key;
  mutable QVariant value;
  QString title;
};

class Resource : public QAbstractListModel {
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  Q_PROPERTY(bool initStatus READ isInitializing NOTIFY initStatusChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
  Q_PROPERTY(QString translation READ translation NOTIFY translationChanged)

 public slots:
  virtual void onTextChange(QString value) {
    // TODO: make this to use another thread
    search(value);
  }

 signals:
  void loadingChanged(bool loading);
  void initStatusChanged(bool initializationStatus);
  void enabledChanged(bool enabled);
  void translationChanged(QString translation);

 public:
  enum roles { ROLE_KEY = Qt::UserRole, ROLE_VALUE, ROLE_TITLE, ROLE_TYPE };
  virtual ~Resource() override {}

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  // Editable:
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

 public:
  explicit Resource(QObject* parent = nullptr);
  virtual void search(QString const& /* data */) {
    // do nothing
  }
  Q_INVOKABLE virtual QString translation() const noexcept {
    return "";  // default value
  }

  /**
   * @brief name of the resource in English or other languages
   * @return QString
   */
  Q_INVOKABLE virtual QString name() const noexcept {
    return tr("UnNamed Translator");
  }

  /**
   * @brief key to use in options
   * @return
   */
  Q_INVOKABLE virtual QString key() const noexcept { return "default"; }

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
  Q_INVOKABLE virtual bool isSupported(
      QOnlineTranslator::Language /* from */,
      QOnlineTranslator::Language /* to */) const noexcept {
    return false;  // default value
  }

  /**
   * @brief check if the resource is enabled or not
   * @return
   */
  Q_INVOKABLE virtual bool isEnabled() const noexcept {
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
    return {};  // empty result
  }

  /**
   * @brief is loading the resource
   * @return
   */
  Q_INVOKABLE bool isLoading() const noexcept { return loading; }

  /**
   * @brief is the resource initializing (for example: connecting to the MySQL
   * database). this funciton will be used in the splash screen to make sure the
   * resource is connected to the database
   * @return
   */
  Q_INVOKABLE bool isInitializing() const noexcept { return initilizing; }

  /**
   * @brief get an option
   * @param _key
   * @param defaultValue
   * @return
   */
  virtual const resource_option& option(QString const& _key) const;

  /**
   * @brief returns a value of an option if exists
   * @param _key
   * @return
   */
  virtual const QVariant& optionValue(
      QString const& _key,
      const QVariant& defaultValue = QVariant()) const;

  /**
   * @brief get a reference to all the options (options are cached in memory)
   * @return
   */
  virtual const QVector<resource_option>& options() const {
    return options_cache;
  }

  /**
   * @brief set an option
   * @param _key
   * @param value
   */
  virtual void setOption(resource_option const& the_option);

  virtual void reloadOptionsCache();

 protected:
  QVector<resource_option> options_cache;
  bool loading = false;
  bool initilizing = true;
};

#endif  // RESOURCE_H
