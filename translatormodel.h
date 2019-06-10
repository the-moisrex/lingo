#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>

class TranslatorModel : public QAbstractListModel {
 public:
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)
  bool loading = true;

 signals:
  void loadingChanged(bool loading);

 public slots:
  void onTextChanged(QString value);

 public:
  enum { wordRole = Qt::UserRole };
  explicit TranslatorModel(QObject* parent = nullptr);
  inline bool isLoading() const noexcept { return loading; }

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  virtual QHash<int, QByteArray> roleNames() const override;

  void search(QString const& data);
};

#endif  // TRANSLATORMODEL_H
