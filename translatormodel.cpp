#include "translatormodel.h"
#include <QDebug>
#include <QOnlineTranslator>

TranslatorModel::TranslatorModel(QObject* parent)
    : QAbstractListModel(parent), translator(this) {}

int TranslatorModel::rowCount(const QModelIndex& parent) const {
  // For list models only the root node (an invalid parent) should return the
  // list's size. For all other (valid) parents, rowCount() should return 0 so
  // that it does not become a tree model.
  if (parent.isValid())
    return 0;

  // FIXME: Implement me!
  return 10;
}

QVariant TranslatorModel::data(const QModelIndex& index, int role) const {
  if (!index.isValid())
    return QVariant();

  switch (role) {
    case sourceRole:
      return QVariant(translator.source());

    case translationRole:
      return QVariant(translator.translation());
  }

  // FIXME: Implement me!
  return QVariant();
}

QHash<int, QByteArray> TranslatorModel::roleNames() const {
  QHash<int, QByteArray> roles;
  roles[sourceRole] = "source";
  roles[translationRole] = "translation";
  return roles;
}

void TranslatorModel::search(const QString& data) {
  translator.translate(data, QOnlineTranslator::Google);
  if (translator.error() == QOnlineTranslator::NoError)
    qInfo() << translator.translation();
  else
    qCritical() << translator.errorString();
  qInfo() << translator.translationTranslit();
}

void TranslatorModel::onTextChanged(QString value) {
  // loading changes so the ui gets affected
  loading = true;
  emit loadingChanged(loading);

  search(value);

  loading = false;
  emit loadingChanged(loading);
}
