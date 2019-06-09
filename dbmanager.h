#ifndef DBRAII_H
#define DBRAII_H

#include <QSqlDatabase>

class DBManager {
 private:
  QSqlDatabase db;
  void createSchema() noexcept;

 public:
  DBManager();
};

#endif  // DBRAII_H
