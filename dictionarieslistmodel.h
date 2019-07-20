#ifndef DICTIONARIESLISTMODEL_H
#define DICTIONARIESLISTMODEL_H

#include <QAbstractListModel>
#include <memory>
#include "resource.h"

class DictionariesListModel : public QAbstractListModel {
  Q_OBJECT
  QList<Resource*> dicts;
  Q_PROPERTY(DictionariesListModel* prototypes READ prototypes NOTIFY
                 prototypesChanged)

  QOnlineTranslator::Language from, to;
  QString lastWord;

  DictionariesListModel* proto = nullptr;
  std::vector<std::tuple<QString, QString, QString>> getManuallyAddedDicts();
  void updateManuallyAddedResources(
      std::vector<std::tuple<QString, QString, QString>> const&
          additional_resources);

 public slots:
  void onTranslationChange(Resource* ptr, QString str);
  void onLoadingChange(Resource* ptr, bool loading);
  void onEnabledChange(Resource* ptr, bool enabled);
  void onInitStatusChange(Resource* ptr, bool initializing);

 signals:
  void prototypesChanged();

 public:
  enum roles {
    NAME = Qt::UserRole,
    TRANSLATION,
    KEY,
    ID,
    DESCRIPTION,
    ENABLED,
    LOADING,
    INITIALIZING,
    INDEX
  };
  explicit DictionariesListModel(QObject* parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void search(QString const& word);
  Q_INVOKABLE Resource* optionsModel(int index) { return dicts.at(index); }
  Q_INVOKABLE static QString readableTranslation(QString t);

  DictionariesListModel* prototypes();
  void loadDefaults();

  Q_INVOKABLE void create(QString name, int index);
  Q_INVOKABLE void remove(QString id);
  Q_INVOKABLE QStringList toLangsModel();
  Q_INVOKABLE QStringList fromLangsModel();
  Q_INVOKABLE void setFromLang(int index);
  Q_INVOKABLE void setToLang(int index);
};

#endif  // DICTIONARIESLISTMODEL_H
