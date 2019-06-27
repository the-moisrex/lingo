#ifndef MY_HISTORY_H
#define MY_HISTORY_H

#include <QList>
#include <QString>

void put_history(QString word);
QList<QString> get_history();
void clear_history();
int history_count();

#endif  // MY_HISTORY_H
