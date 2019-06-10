#include "dbmanager.h"
#include <qdebug.h>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVector>
#include <algorithm>

void DBManager::createSchema() noexcept {
  QSqlQuery query(db);
  QVector<QString> queries;
  queries << "create table if not exists history ("
             "  id integer primary key not null,"
             "  word text not null unique"
             ");";
  queries << "create table if not exists favs ("
             "  id integer primary key not null,"
             "  word text not null unique"
             ");";
  for (auto& q : queries) {
    if (!query.exec(q)) {
      qDebug() << QObject::tr("Error in initilizing database templates: ")
               << query.lastError().driverText();
      break;
    }
  }
}

DBManager::DBManager() : db{QSqlDatabase::addDatabase("QSQLITE", "settings")} {
  db.setDatabaseName("lingo.sqlite");
  if (!db.open()) {
    qDebug() << QObject::tr("Error connecting to database: ") +
                    db.lastError().driverText();
  }

  // checking table names:
  QVector<QString> tableNames;
  tableNames << "history";

  QSqlQuery query(db);
  if (!query.exec("SELECT name FROM sqlite_master WHERE type='table';")) {
    qDebug() << QObject::tr("SQL error, cannot run query: ")
             << db.lastError().driverText();
  }
  int idName = query.record().indexOf("name");
  int correct_tables = 0;
  while (query.next()) {
    QString name = query.value(idName).toString();
    if (std::find(tableNames.cbegin(), tableNames.cend(), name) !=
        tableNames.cend()) {
      correct_tables++;
    }
  }
  if (correct_tables != tableNames.size()) {
    createSchema();
  }
}

DBManager::~DBManager() noexcept {
  db.close();
  QSqlDatabase::removeDatabase("settings");
}

std::shared_ptr<DBManager> db() {
  static std::shared_ptr<DBManager> db;
  if (db)
    return db;
  db = std::make_shared<DBManager>();
  return db;
}
