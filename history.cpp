#include "history.h"

#include <QSqlQuery>
#include "dbmanager.h"

int history_count() {
  QSqlQuery query(db()->db);
  if (query.exec("select count(*) from history")) {
    query.first();
    return query.value(0).toInt();
  }
  return 0;
}

void clear_history() {
  QSqlQuery query(db()->db);
  query.exec("delete from history;");
}

QList<QString> get_history() {
  QSqlQuery query(db()->db);
  QList<QString> hist;
  if (query.exec("select word from history;")) {
    while (query.next()) {
      hist << query.value(0).toString();
    }
  }
  return hist;
}

void put_history(QString word) {
  QSqlQuery query(db()->db);
  query.prepare("insert into history (word) values (?)");
  query.bindValue(0, word);
  query.exec();
}
