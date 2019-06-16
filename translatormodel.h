#ifndef TRANSLATORMODEL_H
#define TRANSLATORMODEL_H

#include <QAbstractListModel>
#include <QOnlineTranslator>

class TranslatorModel : public QAbstractListModel {
 private:
  QOnlineTranslator::Language from = QOnlineTranslator::Language::Auto;
  QOnlineTranslator::Language to = QOnlineTranslator::Language::English;
  QOnlineTranslator translator;
  bool loading = false;

 public:
  Q_OBJECT
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)

 signals:
  void loadingChanged(bool loading);

 public slots:
  void onTextChanged(QString value);

 public:
  enum { sourceRole = Qt::UserRole, translationRole };
  explicit TranslatorModel(QObject* parent = nullptr);
  inline bool isLoading() const noexcept { return loading; }

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index,
                int role = Qt::DisplayRole) const override;

  virtual QHash<int, QByteArray> roleNames() const override;

  void search(QString const& data);
};

#endif  // TRANSLATORMODEL_H
