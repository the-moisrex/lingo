#include "dictionarieslistmodel.h"
#include <QtConcurrent/QtConcurrent>
#include "onlinetranslators.h"

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
    : QAbstractListModel(parent) {
  // Default online dictionaries:
  auto google = new OnlineTranslator<QOnlineTranslator::Google>(this);
  auto bing = new OnlineTranslator<QOnlineTranslator::Bing>(this);
  auto yandex = new OnlineTranslator<QOnlineTranslator::Yandex>(this);
  dicts << google << bing << yandex;

  // Default offline dictionaries:

  // Manually added dictionaries:

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
  data[ENABLED] = "enabled";
  data[LOADING] = "loading";
  data[INITIALIZING] = "initStatus";
  return data;
}

void DictionariesListModel::search(const QString& word) {
  for (auto& dic : dicts) {
    QtConcurrent::run(dic, &Resource::search, word);
  }
}
