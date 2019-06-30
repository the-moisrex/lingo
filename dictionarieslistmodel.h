#ifndef DICTIONARIESLISTMODEL_H
#define DICTIONARIESLISTMODEL_H

#include <QAbstractListModel>
#include "resource.h"

class DictionariesListModel : public QAbstractListModel {
  Q_OBJECT
  QList<Resource*> dicts;

 public slots:
  void onTranslationChange(Resource* ptr, QString str);
  void onLoadingChange(Resource* ptr, bool loading);
  void onEnabledChange(Resource* ptr, bool enabled);
  void onInitStatusChange(Resource* ptr, bool initializing);

 public:
  enum roles {
    NAME = Qt::UserRole,
    TRANSLATION,
    KEY,
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
};

#endif  // DICTIONARIESLISTMODEL_H
