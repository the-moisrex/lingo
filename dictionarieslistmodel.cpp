#include "dictionarieslistmodel.h"
#include <QtConcurrent/QtConcurrent>
#include <algorithm>
#include "onlinetranslators.h"
#include "sqldictionary.h"
#include "txtdictionary.h"

std::vector<std::tuple<QString, QString, QString>>
DictionariesListModel::getManuallyAddedDicts() {
  auto _settings = settings();
  auto size = _settings->beginReadArray("additional_resources");
  std::vector<std::tuple<QString, QString, QString>> additional_resources;
  for (decltype(size) i = 0; i < size; i++) {
    _settings->setArrayIndex(i);
    auto type = _settings->value("type", "").toString();
    auto name = _settings->value("name", "").toString();
    auto id = _settings->value("id", "").toString();
    if (type == "" || name == "" || id == "")
      continue;
    additional_resources.emplace_back(type, id, name);
  }
  _settings->endArray();

  return additional_resources;
}

void DictionariesListModel::updateManuallyAddedResources(
    std::vector<std::tuple<QString, QString, QString>> const&
        additional_resources) {
  auto _settings = settings();

  // re-writing the configs
  auto size = additional_resources.size();
  _settings->beginWriteArray("additional_resources");
  for (decltype(size) i = 0; i < size; i++) {
    auto item = additional_resources.at(i);
    _settings->setArrayIndex(static_cast<int>(i));
    _settings->setValue("type", std::get<0>(item));
    _settings->setValue("id", std::get<1>(item));
    _settings->setValue("name", std::get<2>(item));
  }
  _settings->endArray();
  _settings->sync();

  // disconnect the signals
  for (auto& dic : dicts) {
    disconnect(dic, SIGNAL(translationChanged(Resource*, QString)), this,
               SLOT(onTranslationChange(Resource*, QString)));
    disconnect(dic, SIGNAL(loadingChanged(Resource*, bool)), this,
               SLOT(onLoadingChange(Resource*, bool)));
    disconnect(dic, SIGNAL(enabledChanged(Resource*, bool)), this,
               SLOT(onEnabledChange(Resource*, bool)));
    disconnect(dic, SIGNAL(initStatusChanged(Resource*, bool)), this,
               SLOT(onInitStatusChange(Resource*, bool)));
  }
  dicts.clear();
  loadDefaults();

  emit dataChanged(createIndex(0, 0), createIndex(rowCount(), 0),
                   QVector<int>()
                       << KEY << NAME << TRANSLATION << DESCRIPTION << ENABLED
                       << INDEX << INITIALIZING << LOADING);
}

void DictionariesListModel::loadDefaults() {
  // Default online dictionaries:
  Resource* google = new OnlineTranslator<QOnlineTranslator::Google>(this);
  Resource* bing = new OnlineTranslator<QOnlineTranslator::Bing>(this);
  Resource* yandex = new OnlineTranslator<QOnlineTranslator::Yandex>(this);

  dicts << google;
  dicts << bing;
  dicts << yandex;

  // Default offline dictionaries:
  Resource* english2Espanol =
      new txtDictionary(this, "qrc:/english-spanish-2019-06-25.txt");

  english2Espanol->setName(QObject::tr("English to Spanish (built-in)"));
  dicts << english2Espanol;

  // Manually added dictionaries:
  auto additional_resources = getManuallyAddedDicts();
  for (auto& item : additional_resources) {
    Resource* res = nullptr;
    auto type = std::get<0>(item);
    auto id = std::get<1>(item);
    auto name = std::get<2>(item);

    // Add your other custom dictionaries here:
    if ("txt-dic" == type) {
      res = new txtDictionary(this);
    }
    if (res) {
      res->setId(id);
      res->setName(name);

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
            SLOT(onEnabledChange(Resource*, bool)));
    connect(dic, SIGNAL(initStatusChanged(Resource*, bool)), this,
            SLOT(onInitStatusChange(Resource*, bool)));
  }
}

void DictionariesListModel::create(QString name, int index) {
  if (!proto)
    return;

  auto item = proto->index(index);
  auto type = item.data(KEY).toString();
  auto id =
      QString(QCryptographicHash::hash((type + name).toStdString().c_str(),
                                       QCryptographicHash::Md5)
                  .toHex());
  auto additional_resources = getManuallyAddedDicts();
  additional_resources.emplace_back(type, id, name);
  updateManuallyAddedResources(additional_resources);
}

void DictionariesListModel::remove(QString id) {
  auto additional_resources = getManuallyAddedDicts();
  auto found =
      std::find_if(additional_resources.begin(), additional_resources.end(),
                   [&](auto const& dict) { return std::get<1>(dict) == id; });
  if (found != additional_resources.end()) {
    additional_resources.erase(found);
    updateManuallyAddedResources(additional_resources);
  }
}

QStringList DictionariesListModel::toLangsModel() {
  static QStringList to;
  if (to.empty()) {
    for (int lang = QOnlineTranslator::Language::Afrikaans;
         lang != QOnlineTranslator::Language::Zulu; lang++)
      to.push_back(QOnlineTranslator::languageString(
          static_cast<QOnlineTranslator::Language>(lang)));
  }
  return to;
}

QStringList DictionariesListModel::fromLangsModel() {
  static QStringList from;
  if (from.empty()) {
    for (int lang = QOnlineTranslator::Language::Auto;
         lang != QOnlineTranslator::Language::Zulu; lang++)
      from.push_back(QOnlineTranslator::languageString(
          static_cast<QOnlineTranslator::Language>(lang)));
  }
  return from;
}

void DictionariesListModel::setFromLang(int index) {
  index -= 1;
  from = static_cast<QOnlineTranslator::Language>(index);
  search(lastWord);
}

void DictionariesListModel::setToLang(int index) {
  index -= 2;
  to = static_cast<QOnlineTranslator::Language>(index);
  search(lastWord);
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
    case ID:
      return dic->id();
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
  lastWord = word;
  for (auto& dic : dicts) {
    if (dic->isSupported(from, to)) {
      QtConcurrent::run(dic, &Resource::search, word);
    } else {
      dic->clearTranslation();
    }
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
