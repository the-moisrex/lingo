#include "resourceoptions.h"
#include <algorithm>
#include "resource.h"

ResourceOptionsModel::ResourceOptionsModel(QObject* parent)
    : QAbstractListModel(parent) {}

int ResourceOptionsModel::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  return _options.size();
}

QVariant ResourceOptionsModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto const& opt = _options.at(index.row());

  switch (role) {
    case ROLE_KEY:
      return opt.key;
    case ROLE_TYPE:
      return opt.input_type;
    case ROLE_TITLE:
      return opt.title;
    case ROLE_VALUE:
      return opt.value;
  }

  return QVariant();
}

QMap<int, QVariant> ResourceOptionsModel::itemData(
    const QModelIndex& /* index */) const {
  QMap<int, QVariant> data;
  data[ROLE_KEY] = "key";
  data[ROLE_TYPE] = "type";
  data[ROLE_TITLE] = "title";
  data[ROLE_VALUE] = "value";
  return data;
}

bool ResourceOptionsModel::setData(const QModelIndex& index,
                                   const QVariant& value,
                                   int role) {
  if (data(index, role) != value) {
    auto const& opt = _options.at(index.row());
    switch (role) {
      case ROLE_VALUE:  // only value is mutable and thus makes scense to change
        opt.value = value;
        setOption(opt);
        break;
      default:
        return false;
    }
    emit dataChanged(index, index, QVector<int>() << role);
    return true;
  }
  return false;
}

Qt::ItemFlags ResourceOptionsModel::flags(const QModelIndex& index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEditable;  // FIXME: Implement me!
}

QVector<resource_option>::const_iterator ResourceOptionsModel::option(
    const QString& key) const noexcept {
  return std::find_if(_options.begin(), _options.end(),
                      [&](auto const& a) { return a.key == key; });
}

void ResourceOptionsModel::setOption(const resource_option& value) {
  auto opt = option(value.key);
  auto p = parent();
  Resource* res = nullptr;
  if (p)
    res = dynamic_cast<Resource*>(p);
  if (opt == _options.end()) {
    _options.push_back(value);
    if (res)
      res->setOption(value);

  } else {
    opt->value = value.value;
    if (res)
      res->setOption(*opt);
  }
  // TODO: I don't think we need "_options" anymore
}
