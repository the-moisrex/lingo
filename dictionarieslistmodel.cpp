#include "dictionarieslistmodel.h"
#include "onlinetranslators.h"

DictionariesListModel::DictionariesListModel(QObject* parent)
    : QAbstractListModel(parent) {
  // Default online dictionaries:
  auto google = new OnlineTranslator<QOnlineTranslator::Google>(this);
  auto bing = new OnlineTranslator<QOnlineTranslator::Bing>(this);
  auto yandex = new OnlineTranslator<QOnlineTranslator::Yandex>(this);
  dicts << google << bing << yandex;

  // Default offline dictionaries:

  // Manually added dictionaries:
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
  for (auto& dic : dicts)
    dic->search(word);
}
