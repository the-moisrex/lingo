#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>
#include <QtCore>

class Translator : public QObject {
  Q_OBJECT
  Q_PROPERTY(QStringListModel resultsModel READ getResultsModel NOTIFY
                 resultsModelChanged)

 private:
  QStringListModel* resultsModel;

 public:
  explicit Translator(QObject* parent = nullptr);
  inline QStringListModel* getResultsModel() const noexcept {
    return resultsModel;
  }
};

#endif  // TRANSLATOR_H
