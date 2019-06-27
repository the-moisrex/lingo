#ifndef DICTIONARIESLISTMODEL_H
#define DICTIONARIESLISTMODEL_H

#include <QAbstractListModel>
#include "resource.h"

class DictionariesListModel : public QAbstractListModel {
  Q_OBJECT
  QList<Resource*> dicts;

 public:
  enum roles {
    NAME = Qt::UserRole,
    TRANSLATION,
    KEY,
    DESCRIPTION,
    ENABLED,
    LOADING,
    INITIALIZING
  };
  explicit DictionariesListModel(QObject* parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QHash<int, QByteArray> roleNames() const override;

  Q_INVOKABLE void search(QString const& word);
};

#endif  // DICTIONARIESLISTMODEL_H
