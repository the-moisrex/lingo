#include "sqldictionary.h"
#include <QReadLocker>
#include <QSqlError>
#include <QSqlQuery>
#include <QWriteLocker>
#include <QtConcurrent/QtConcurrent>

void SqlDictionary::concurentSearch(QString word) {
  QString result;
  QSqlQuery query(db);
  if (dbinfo.customSql.size() == 0 && dbinfo.tableName.size() != 0 &&
      dbinfo.langOne.size() != 0 && dbinfo.langTwo.size() != 0) {
    query.prepare(
        "select :langTwo from :tableName where :LangOne = :word limit 1;");
    query.bindValue(":langOne", dbinfo.langOne);
    query.bindValue(":langTwo", dbinfo.langTwo);
    query.bindValue(":tableName", dbinfo.tableName);
    query.bindValue(":word", word);
  } else if (dbinfo.customSql.size() != 0) {
    query.prepare(dbinfo.customSql);
    query.bindValue(0, word);  // uses "?" character
  } else {
    clearTranslation();
    setLoading(false);
    return;
  }

  if (query.exec()) {
    if (query.next())
      result = query.value(0).toString();
  } else {
    clearTranslation();
    setLoading(false);
    return;
  }

  {
    QReadLocker locked(&currentWordLock);
    if (currentWord == word && result.size() != 0) {
      setTranslation(std::move(result));
    } else if (currentWord == word && result.size() == 0) {
      clearTranslation();
    } else {
      // nothing to do, translation is being handled by another concurent
      // process. we do not even want to cancel the loading.
      return;
    }
  }

  setLoading(false);
}

void SqlDictionary::search(const QString& word) {
  QString _word = word.trimmed().toLower();
  {
    QReadLocker locked(&currentWordLock);
    if (_word == currentWord)
      return;
  }
  {
    QWriteLocker locked(&currentWordLock);
    currentWord = _word;
  }
  setLoading(true);
  QtConcurrent::run(this, &SqlDictionary::concurentSearch, _word);
}

void SqlDictionary::componentComplete() {
  Resource::componentComplete();

  // Setting the options
  setOptionIfNotExists({resource_option::TEXT, "hostname", "", "Host name"});
  // TODO: add more options

  // init
  setInitStatus(true);
  QtConcurrent::run(this, &SqlDictionary::init);
}
