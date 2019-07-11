#include "dictionarieslistmodel.h"
#include <QtConcurrent/QtConcurrent>
#include <algorithm>
#include "onlinetranslators.h"
#include "sqldictionary.h"
#include "txtdictionary.h"

void DictionariesListModel::loadDefaults() {
  // Default online dictionaries:
  Resource* google = new OnlineTranslator<QOnlineTranslator::Google>(this);
  Resource* bing = new OnlineTranslator<QOnlineTranslator::Bing>(this);
  Resource* yandex = new OnlineTranslator<QOnlineTranslator::Yandex>(this);

  if (google->isEnabled())
    dicts << google;
  if (bing->isEnabled())
    dicts << bing;
  if (yandex->isEnabled())
    dicts << yandex;

  // Default offline dictionaries:
  Resource* english2Espanol =
      new txtDictionary(this, "qrc:/english-spanish-2019-06-25.txt");

  english2Espanol->setName(QObject::tr("English to Spanish (built-in)"));
  if (english2Espanol->isEnabled())
    dicts << english2Espanol;

  // Manually added dictionaries:
  auto _settings = settings();
  auto size = _settings->beginReadArray("additional_resources");
  std::vector<std::tuple<QString, QString, QString>> additional_resources;
  for (decltype(size) i = 0; i < size; i++) {
    _settings->setArrayIndex(i);
    auto type = _settings->value("type", "").toString();
    auto name = _settings->value("name", "").toString();
    auto key = _settings->value("key", "").toString();
    if (type == "" || name == "" || key == "")
      continue;
    additional_resources.emplace_back(type, key, name);
  }
  _settings->endArray();

  for (auto& item : additional_resources) {
    Resource* res = nullptr;
    auto type = std::get<0>(item);
    auto key = std::get<1>(item);
    auto name = std::get<2>(item);

    // Add your other custom dictionaries here:
    if ("txt-dic" == type) {
      res = new txtDictionary(this);
    }
    if (res) {
      res->setId(key);
      res->setName(name);

      if (res->isEnabled())
        dicts << res;
    }
  }

  // connect the signals to the slots
  for (auto& dic : dicts) {
    connect(dic, SIGNAL(translationChanged(Resource*, QString)), this,
            SLOT(onTranslationChange(Resource*, QString)));
    connect(dic, SIGNAL(loadingChanged(Resource*, bool)), this,
            SLOT(onLoadingChange(Resource*, bool)));
    connect(dic, SIGNAL(enabledChanged(Resource*, bool)), this,
            SLOT(onLoadingChange(Resource*, bool)));
    connect(dic, SIGNAL(initStatusChanged(Resource*, bool)), this,
            SLOT(onInitStatusChange(Resource*, bool)));
  }
}

void DictionariesListModel::create(QString name, int index) {
  if (!proto)
    return;

  auto item = proto->index(index);
  auto type = item.data(KEY).toString();
  auto key =
      QString(QCryptographicHash::hash((type + name).toStdString().c_str(),
                                       QCryptographicHash::Md5)
                  .toHex());
  auto _settings = settings();

  auto size = _settings->value("aadditional_resource", 0).toInt();
  _settings->beginWriteArray("additional_resources");
  _settings->setArrayIndex(size + 1);
  _settings->setValue("key", key);
  _settings->setValue("type", type);
  _settings->setValue("name", name);
  _settings->endArray();
  _settings->sync();
}

void DictionariesListModel::onTranslationChange(Resource* ptr,
                                                QString /* str */) {
  int index = dicts.indexOf(ptr);
  if (index < 0)
    return;
  emit dataChanged(createIndex(index, 0), createIndex(index, 0),
                   QVector<int>() << TRANSLATION);
}

void DictionariesListModel::onLoadingChange(Resource* ptr, bool /* loading */) {
  int index = dicts.indexOf(ptr);
  if (index < 0)
    return;
  emit dataChanged(createIndex(index, 0), createIndex(index, 0),
                   QVector<int>() << LOADING);
}

void DictionariesListModel::onEnabledChange(Resource* ptr, bool /* enabled */) {
  int index = dicts.indexOf(ptr);
  if (index < 0)
    return;
  emit dataChanged(createIndex(index, 0), createIndex(index, 0),
                   QVector<int>() << ENABLED);
}

void DictionariesListModel::onInitStatusChange(Resource* ptr,
                                               bool /* initializing */) {
  int index = dicts.indexOf(ptr);
  if (index < 0)
    return;
  emit dataChanged(createIndex(index, 0), createIndex(index, 0),
                   QVector<int>() << INITIALIZING);
}

DictionariesListModel::DictionariesListModel(QObject* parent)
    : QAbstractListModel(parent) {}

int DictionariesListModel::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  return dicts.size();
}

QVariant DictionariesListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  auto dic = dicts.at(index.row());

  switch (role) {
    case KEY:
      return dic->key();
    case NAME:
      return dic->name();
    case TRANSLATION:
      return dic->translation();
    case ENABLED:
      return dic->isEnabled();
    case DESCRIPTION:
      return dic->description();
    case LOADING:
      return dic->isLoading();
    case INITIALIZING:
      return dic->isInitializing();
    case INDEX:
      return index.row();
  }

  return QVariant();
}

QHash<int, QByteArray> DictionariesListModel::roleNames() const {
  QHash<int, QByteArray> data;
  data[KEY] = "key";
  data[NAME] = "name";
  data[DESCRIPTION] = "description";
  data[TRANSLATION] = "translation";
  data[ENABLED] = "translatorEnabled";
  data[LOADING] = "loading";
  data[INITIALIZING] = "initStatus";
  return data;
}

void DictionariesListModel::search(const QString& word) {
  for (auto& dic : dicts) {
    QtConcurrent::run(dic, &Resource::search, word);
  }
}

QString DictionariesListModel::readableTranslation(QString t) {
  return t.remove(QRegExp("<[^>]*>"));
}

DictionariesListModel* DictionariesListModel::prototypes() {
  if (!proto) {
    proto = new DictionariesListModel(this);
    proto->dicts << new txtDictionary(proto);
  }
  return proto;
}
