#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>

class TranslatorModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit TranslatorModel(QObject* parent = nullptr);

  enum roles {

  };

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

 private:
};

#endif  // TRANSLATORMODEL_H
