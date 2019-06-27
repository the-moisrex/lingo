#ifndef DBRAII_H
#define DBRAII_H

#include <QSqlDatabase>
#include <QSqlTableModel>
#include <memory>

class DBManager {
 private:
  void createSchema() noexcept;

 public:
  QSqlDatabase db;
  DBManager();
  ~DBManager() noexcept;
};

std::shared_ptr<DBManager> db();

#endif  // DBRAII_H
