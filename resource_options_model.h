#ifndef RESOURCE_OPTIONS_H
#define RESOURCE_OPTIONS_H

#include <QAbstractListModel>
#include <QString>
#include <QVariant>

struct resource_option {
  enum input_t { TEXT = 1, TEXT_LONG, CHECKBOX, MULTICHOICE, OPTIONS_SWITCHER };
  input_t input_type;
  QString key;
  mutable QVariant value;
  QString title;
  QStringList choices = {}; // for multichoice
  QMap<QString, QVector<resource_option>> available_options =
      {}; // for options_switcher

  inline bool operator==(resource_option const &opt) const noexcept {
    return this->input_type == opt.input_type && key == opt.key &&
           value == opt.value && title == opt.title && choices == opt.choices &&
           available_options == opt.available_options;
  }
};

enum roles {
  ROLE_KEY = Qt::UserRole,
  ROLE_VALUE,
  ROLE_TITLE,
  ROLE_TYPE,
  ROLE_CHOICES,
  ROLE_OPTIONS_SWITCHER
};

class ResourceOptionsModel : public QAbstractListModel {
  Q_OBJECT

  QVector<resource_option> options;

public:
  explicit ResourceOptionsModel(QObject *parent = nullptr);
  ResourceOptionsModel(QObject *parent, decltype(options) _options);

  // Basic functionality:
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;

  QHash<int, QByteArray> roleNames() const override;

  // Editable:
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override;

  Qt::ItemFlags flags(const QModelIndex &index) const override;
};

#endif // RESOURCE_OPTIONS_H
