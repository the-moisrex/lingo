#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include <QObject>

class Translator : public QObject
{
  Q_OBJECT
public:
  explicit Translator(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TRANSLATOR_H
