#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QtCore>

class Translator : public QObject {
  Q_OBJECT
  Q_PROPERTY(QStringListModel resultsModel READ getResultsModel NOTIFY
                 resultsModelChanged)
  Q_PROPERTY(bool loading READ isLoading NOTIFY loadingChanged)

 private:
  QStringListModel* resultsModel;
  bool loading = true;

 public:
  explicit Translator(QObject* parent = nullptr);
  inline QStringListModel* getResultsModel() const noexcept {
    return resultsModel;
  }
  inline bool isLoading() const noexcept { return loading; }

 signals:
  void resultsModelChanged(QStringListModel* resultsModel);
  void loadingChanged(bool loading);

 public slots:
  void onTextChanged(QString value);
};

#endif  // TRANSLATOR_H
