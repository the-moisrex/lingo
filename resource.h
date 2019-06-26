#ifndef RESOURCE_H
#define RESOURCE_H

#include <QObject>
#include <QOnlineTranslator>
#include <QString>
#include <utility>
#include "resourceoptions.h"

class Resource : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  Q_PROPERTY(bool initStatus READ isInitializing NOTIFY initStatusChanged)
  Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
  Q_PROPERTY(QString translation READ translation NOTIFY translationChanged)

 public slots:
  virtual void onTextChange(QString value) { search(value); }

 signals:
  void loadingChanged(bool loading);
  void initStatusChanged(bool initializationStatus);
  void enabledChanged(bool enabled);
  void translationChanged(QString translation);

 public:
  explicit Resource(QObject* parent = nullptr) : QObject(parent) {}
  virtual ~Resource() noexcept;
  virtual void search(QString const& data);
  virtual QString translation() const noexcept;
  virtual QString name() const noexcept;
  virtual QString description() const noexcept;
  virtual bool isSupported(QOnlineTranslator::Language from,
                           QOnlineTranslator::Language to) const noexcept;
  virtual bool isEnabled() const noexcept;
  virtual bool setEnabled() noexcept;
  virtual bool canProvideSuggestions() const noexcept { return false; }
  virtual QVector<QString> suggestions() const noexcept;
  bool isLoading() const noexcept { return loading; }
  bool isInitializing() const noexcept { return initilizing; }
  virtual ResourceOptionsModel optionsModel() const noexcept;

 protected:
  bool loading = false;
  bool initilizing = true;
};

#endif  // RESOURCE_H
