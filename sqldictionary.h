#ifndef SQLDICTIONARY_H
#define SQLDICTIONARY_H

#include <QObject>
#include "resource.h"

class SqlDictionary : public Resource {
  Q_OBJECT

 public:
  explicit SqlDictionary(QObject* parent) : Resource(parent) {}

  void search(const QString&) override;
};

#endif  // SQLDICTIONARY_H
