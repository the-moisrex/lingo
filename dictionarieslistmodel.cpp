#include "dictionarieslistmodel.h"
#include "onlinetranslators.h"

DictionariesListModel::DictionariesListModel(QObject* parent)
    : QAbstractListModel(parent) {
  // Default online dictionaries:
  //  OnlineTranslator<QOnlineTranslator::Google> google;
  //  OnlineTranslator<QOnlineTranslator::Bing> bing;
  //  OnlineTranslator<QOnlineTranslator::Yandex> yandex;
  //  dicts << std::move(google) << std::move(bing) << std::move(yandex);

  // Default offline dictionaries:

  // Manually added dictionaries:
}

int DictionariesListModel::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  //  return dicts.size();
}

QVariant DictionariesListModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  switch (role) {}

  return QVariant();
}
