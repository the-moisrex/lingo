#include "resource.h"
#include <QDebug>

int Resource::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  return options_cache.size();
}

QVariant Resource::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto const& opt = options_cache.at(index.row());

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

QHash<int, QByteArray> Resource::roleNames() const {
  QHash<int, QByteArray> data;
  data[ROLE_KEY] = "key";
  data[ROLE_TYPE] = "type";
  data[ROLE_TITLE] = "title";
  data[ROLE_VALUE] = "value";
  return data;
}

bool Resource::setData(const QModelIndex& index,
                       const QVariant& value,
                       int role) {
  if (data(index, role) != value) {
    auto const& opt = options_cache.at(index.row());
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

Qt::ItemFlags Resource::flags(const QModelIndex& index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return Qt::ItemIsEditable;  // FIXME: Implement me!
}

void Resource::componentComplete() {
  // adding enabled option because it'll going to be common among all
  // dictionaries
  if (!optionExists("enabled"))
    setOption({resource_option::CHECKBOX, "enabled", true, tr("Enabled")});
}

void Resource::classBegin() {}

Resource::Resource(QObject* parent) : QAbstractListModel(parent) {}

void Resource::setEnabled(bool enabled) {
  QWriteLocker locker(&enabledLock);
  resource_option opt = option("enabled");
  opt.value = enabled;
  setOption(opt);
  emit enabledChanged(this, enabled);
}

const resource_option& Resource::option(const QString& _key) const {
  auto opts = options();
  for (auto const& opt : opts)
    if (opt.key == _key)
      return opt;
  throw std::invalid_argument(
      "The specified key is not in the configutation file");
}

bool Resource::optionExists(const QString& _key) const {
  auto opts = options();
  for (auto const& opt : opts)
    if (opt.key == _key)
      return true;
  return false;
}

QVariant Resource::optionValue(const QString& _key,
                               const QVariant& defaultValue) const {
  auto opts = options();
  for (auto const& opt : opts)
    if (opt.key == _key)
      return opt.value;

  return defaultValue;
}

const QVector<resource_option>& Resource::options() const {
  if (options_cache.empty()) {
    reloadOptionsCache();
  }
  return options_cache;
}

void Resource::setOption(const resource_option& the_option) {
  auto _settings = settings();
  auto opts = options();
  bool should_add = true;
  for (auto it = opts.begin(); it != opts.end(); it++) {
    if (it->key == the_option.key) {
      options_cache.replace(static_cast<int>(it - opts.begin()), the_option);
      _settings->beginWriteArray(id() + "/options", opts.size());
      int i = 0;
      int size = opts.size();
      for (; i < size; i++) {
        _settings->setArrayIndex(i);
        if (_settings->value("key", "").toString() == the_option.key) {
          _settings->setValue("value", the_option.value);
          _settings->setValue("input_type",
                              static_cast<int>(the_option.input_type));
          _settings->setValue("title", the_option.title);
          should_add = false;

          if (the_option.key == "enabled")
            emit enabledChanged(this, the_option.value.toBool());

          break;
        }
      }
      _settings->endArray();
      break;
    }
  }

  // "the_option" does not exists, so we have to add it
  if (should_add) {
    options_cache.push_back(the_option);
    _settings->beginWriteArray(id() + "/options", opts.size());
    _settings->setArrayIndex(opts.size());
    _settings->setValue("key", the_option.key);
    _settings->setValue("value", the_option.value);
    _settings->setValue("input_type", static_cast<int>(the_option.input_type));
    _settings->setValue("title", the_option.title);
    _settings->endArray();
  }
  _settings->sync();
}

void Resource::reloadOptionsCache() const {
  // cache all the options:
  auto _settings = settings();
  auto size = _settings->beginReadArray(id() + "/options");
  options_cache.clear();
  options_cache.reserve(size);
  for (int i = 0; i < size; ++i) {
    _settings->setArrayIndex(i);
    options_cache.push_back({static_cast<resource_option::input_t>(
                                 _settings->value("input_type").toInt()),
                             _settings->value("key").toString(),
                             _settings->value("value").toString(),
                             _settings->value("title").toString()});
  }
  _settings->endArray();
}
