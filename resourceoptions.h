#ifndef RESOURCEOPTIONS_H
#define RESOURCEOPTIONS_H

#include <QAbstractListModel>
#include <QString>
#include <QVariant>

struct option {
  enum input_t { TEXT, CHECKBOX } input_type;
  QString key;
  QVariant value;
  QString title;
};

class ResourceOptionsModel : public QAbstractListModel {
  Q_OBJECT

  QVector<option> _options;

 public:
  enum roles { ROLE_KEY = Qt::UserRole, ROLE_VALUE, ROLE_TITLE, ROLE_TYPE };

  explicit ResourceOptionsModel(QObject* parent = nullptr);

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;
  QMap<int, QVariant> itemData(const QModelIndex& index) const override;

  // Editable:
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex& index) const override;

  // Option functionalities:
  const QVector<struct option>& options() const noexcept { return _options; }
  QVector<struct option>::const_iterator option(const QString& key) const
      noexcept;
  void setOption(const struct option& value);
  void setOption(struct option&& value);
};

#endif  // RESOURCEOPTIONS_H
