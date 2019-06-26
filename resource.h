#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>
#include <QOnlineTranslator>
#include <QString>
#include <utility>
#include "resourceoptions.h"
#include "settings.h"

class Resource : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  Q_PROPERTY(bool initStatus READ isInitializing NOTIFY initStatusChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
  Q_PROPERTY(QString translation READ translation NOTIFY translationChanged)
  Q_PROPERTY(ResourceOptionsModel optionsListModel READ optionsModel)

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
  explicit Resource(QObject* parent = nullptr) : QObject(parent), model(this) {
    // adding enabled option because it'll going to be common among all
    // dictionaries
    model.setOption({option::CHECKBOX, "enabled", true, tr("Enabled")});
  }
  virtual ~Resource() noexcept;
  virtual void search(QString const& data);
  Q_INVOKABLE virtual QString translation() const noexcept;

  /**
   * @brief name of the resource in English or other languages
   * @return QString
   */
  Q_INVOKABLE virtual QString name() const noexcept;

  /**
   * @brief key to use in options
   * @return
   */
  Q_INVOKABLE virtual QString key() const noexcept;

  /**
   * @brief description of the resouce
   * @return QString
   */
  Q_INVOKABLE virtual QString description() const noexcept;

  /**
   * @brief checks if the specified languages supported for this resource or not
   * @param from a language
   * @param to a language
   * @return bool
   */
  Q_INVOKABLE virtual bool isSupported(QOnlineTranslator::Language from,
                                       QOnlineTranslator::Language to) const
      noexcept;

  /**
   * @brief check if the resource is enabled or not
   * @return
   */
  Q_INVOKABLE virtual bool isEnabled() const noexcept {
    return option("enabled", true).toBool();
  }

  /**
   * @brief changes the enabled option
   * @return
   */
  Q_INVOKABLE virtual void setEnabled(bool enabled) noexcept {
    setOption("enabled", enabled);
  }

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
  Q_INVOKABLE virtual QVector<QString> suggestions() const noexcept;

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
   * @brief returns options model so the user can edit editable options
   * @return
   */
  Q_INVOKABLE virtual const ResourceOptionsModel& optionsModel() const
      noexcept {
    return model;
  }

 protected:
  ResourceOptionsModel model;
  bool loading = false;
  bool initilizing = true;

  /**
   * @brief get an option
   * @param _key
   * @param defaultValue
   * @return
   */
  virtual QVariant option(QString const& _key,
                          QVariant const defaultValue = QVariant()) const {
    return settings()->value(key() + "/" + _key, defaultValue);
  }

  /**
   * @brief set an option
   * @param _key
   * @param value
   */
  virtual void setOption(QString const& _key, QVariant const& value) {
    settings()->setValue(key() + "/" + _key, value);
  }
};

#endif  // RESOURCE_H
