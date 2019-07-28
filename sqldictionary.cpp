#include "sqldictionary.h"
#include <QCryptographicHash>
#include <QReadLocker>
#include <QSqlError>
#include <QSqlQuery>
#include <QWriteLocker>
#include <QtConcurrent/QtConcurrent>

void SqlDictionary::concurentSearch(QString word) {
  QString result;
  setLoading(true);

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

void SqlDictionary::init() {
  setInitStatus(true);

  if (db.isValid() && db.isOpen()) {
    setInitStatus(false);
    return;
  }

  db.setHostName(dbinfo.hostname);
  db.setPort(dbinfo.port);
  db.setUserName(dbinfo.username);
  db.setPassword(dbinfo.password);
  db.setDatabaseName(dbinfo.dbname);

  if (!db.open()) {
    setTempEnabled(false);
    qDebug() << "Could not open database connection.";
  }

  setInitStatus(false);
}

SqlDictionary::SqlDictionary(QObject* parent, const DatabaseInfo& info)
    : Resource(parent), dbinfo(info) {
  //  QString allinfo;
  //  allinfo.append(dbinfo.type);
  //  allinfo.append(dbinfo.dbname);
  //  allinfo.append(dbinfo.username);
  //  allinfo.append(dbinfo.port);
  //  allinfo.append(dbinfo.hostname);
  //  // TODO: I probably should change this above part. and for txtDictionary
  //  too setId(QString(QCryptographicHash::hash(allinfo.toStdString().c_str(),
  //                                         QCryptographicHash::Md5)
  //                    .toHex()));
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
  QtConcurrent::run(this, &SqlDictionary::concurentSearch, _word);
}

void SqlDictionary::componentComplete() {
  Resource::componentComplete();

  // Setting the options
  setOptionIfNotExists({
      resource_option::MULTICHOICE,
      "database_type",  // key
      7,                // value
      tr("Driver"),     // title
      {tr("QDB2"), tr("QIBASE"), tr("QMYSQL"), tr("QOCI"), tr("QODBC"),
       tr("QPSQL"), tr("QSQLITE2"), tr("QSQLITE"), tr("QTDS")},  // choices
      {}  // options_switcher
  });
  setOptionIfNotExists({
      resource_option::TEXT,
      "hostname",       // key
      "localhost",      // default value
      tr("Host Name"),  // title
  });
  setOptionIfNotExists({
      resource_option::TEXT,
      "port",     // key
      0,          // value
      tr("Port")  // title
  });
  setOptionIfNotExists({
      resource_option::TEXT,
      "username",     // key
      "root",         // default value
      tr("Username")  // title
  });
  setOptionIfNotExists({
      resource_option::TEXT,
      "password",     // key
      "",             // default value
      tr("Password")  // title
  });
  setOptionIfNotExists({
      resource_option::OPTIONS_SWITCHER,
      "query_type",  // key
      0,             // value
      "Query Type",  // Title
      {},            // choices
      {{tr("Simple Table"),
        {{
             resource_option::TEXT,
             "table_name",     // key
             "",               // default value
             tr("Table Name")  // title
         },
         {
             resource_option::TEXT,
             "col1",         // key
             "",             // default value
             tr("Column 1")  // title
         },
         {
             resource_option::TEXT,
             "col2",         // key
             "",             // default value
             tr("Column 2")  // title
         }}},
       {tr("Custom Query"),
        {{
            resource_option::TEXT_LONG,
            "custom_query",                        // key
            "select es from words where en = ?;",  // default query
            tr("Custom Query")                     // title
        }}}},                                      // options_switcher
  });

  // init
  QtConcurrent::run(this, &SqlDictionary::init);
}
