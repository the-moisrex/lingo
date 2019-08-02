#include "resource_options_model.h"
#include <QDebug>

ResourceOptionsModel::ResourceOptionsModel(QObject *parent)
    : QAbstractListModel(parent) {}

ResourceOptionsModel::ResourceOptionsModel(
    QObject *parent, decltype(ResourceOptionsModel::options) _options)
    : QAbstractListModel(parent), options(_options) {}

int ResourceOptionsModel::rowCount(const QModelIndex &parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  return options->size();
}

QVariant ResourceOptionsModel::data(const QModelIndex &index, int role) const {

  if (!index.isValid())
    return QVariant();

  auto const &opt = options->at(index.row());

  switch (role) {
  case ROLE_KEY:
    return opt.key;
  case ROLE_TYPE:
    return opt.input_type;
  case ROLE_TITLE:
    return opt.title;
  case ROLE_VALUE:
    return opt.value;
  case ROLE_CHOICES:
    return opt.choices;
  case ROLE_OPTIONS_SWITCHER:
    QStringList list;
    for (auto &item : opt.available_options.keys())
      list.push_back(item);
    return list;
  }

  return QVariant();
}

QHash<int, QByteArray> ResourceOptionsModel::roleNames() const {
  QHash<int, QByteArray> data;
  data[ROLE_KEY] = "key";
  data[ROLE_TYPE] = "type";
  data[ROLE_TITLE] = "title";
  data[ROLE_VALUE] = "value";
  data[ROLE_CHOICES] = "choices";
  data[ROLE_OPTIONS_SWITCHER] = "available_options";
  return data;
}

bool ResourceOptionsModel::setData(const QModelIndex &index,
                                   const QVariant &value, int role) {
  if (data(index, role) != value) {
    // FIXME: Implement me!
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags ResourceOptionsModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEditable; // FIXME: Implement me!
}
