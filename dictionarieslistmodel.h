#ifndef DICTIONARIESLISTMODEL_H
#define DICTIONARIESLISTMODEL_H

#include <QAbstractListModel>
//#include "resource.h"

class DictionariesListModel : public QAbstractListModel {
  Q_OBJECT

  //  QList<Resource> dicts;

 public:
  explicit DictionariesListModel(QObject* parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 private:
};

#endif  // DICTIONARIESLISTMODEL_H
