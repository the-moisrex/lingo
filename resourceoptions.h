#ifndef RESOURCEOPTIONS_H
#define RESOURCEOPTIONS_H

#include <QAbstractListModel>

struct option {
  QString title;
  QString name;
  enum class input_t { TEXT, CHECKBOX } input_type;
  QVariant value;
};

class ResourceOptionsModel : public QAbstractListModel {
  Q_OBJECT

 public:
  explicit ResourceOptionsModel(QObject* parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  // Editable:
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

 private:
};

#endif  // RESOURCEOPTIONS_H
