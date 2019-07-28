#ifndef SQLDICTIONARY_H
#define SQLDICTIONARY_H

#include <QObject>
#include <QReadWriteLock>
#include "resource.h"

struct DatabaseInfo {
  QString type;
  QString hostname;
  QString dbname;
  QString username;
  QString password;

  QString tableName;
  QString langOne;
  QString langTwo;

  QString customSql;
};

class SqlDictionary : public Resource {
  Q_OBJECT
  DatabaseInfo dbinfo;
  QSqlDatabase db;

  QReadWriteLock currentWordLock;
  QString currentWord;
  void concurentSearch(QString word);
  void init();

 public:
  explicit SqlDictionary(QObject* parent) : Resource(parent) {}
  explicit SqlDictionary(QObject* parent, DatabaseInfo const& info)
      : Resource(parent), dbinfo(info) {}

  QString key() const noexcept override { return "sql-dic"; }

  QString description() const noexcept override {
    return QObject::tr(
        "There are two ways to configure. First, you can you specify table "
        "name, and the names of column one including the words, and column two "
        "including the translation of those words. Second, you can write a "
        "custom select query that will select the translation in the first "
        "column and you need to use question mark(?) as the input word.");
  }

  void search(const QString&) override;
  void componentComplete() override;
};

#endif  // SQLDICTIONARY_H
