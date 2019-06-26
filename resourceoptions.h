#ifndef RESOURCEOPTIONS_H
#define RESOURCEOPTIONS_H

#include <QAbstractListModel>
#include <QString>
#include <QVariant>

struct resource_option {
  enum input_t { TEXT = 1, TEXT_LONG, CHECKBOX };
  input_t input_type;
  QString key;
  mutable QVariant value;
  QString title;
};

class ResourceOptionsModel : public QAbstractListModel {
  Q_OBJECT

  QVector<resource_option> _options;

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

  const QVector<resource_option>& options() const noexcept { return _options; }
  QVector<resource_option>::const_iterator option(const QString& key) const
      noexcept;

  /**
   * @brief set option. will change the value of the option if the option
   * already exists. but only the value
   * @param value
   */
  void setOption(const resource_option& value);
};

#endif  // RESOURCEOPTIONS_H
