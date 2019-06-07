#include "translatormodel.h"

TranslatorModel::TranslatorModel(QObject* parent)
    : QAbstractListModel(parent) {}

int TranslatorModel::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  // FIXME: Implement me!
}

QVariant TranslatorModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  // FIXME: Implement me!
  return QVariant();
}

QHash<int, QByteArray> TranslatorModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[wordRole] = "word";
  return roles;
}