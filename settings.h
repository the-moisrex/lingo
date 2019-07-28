#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <memory>
#include "dbmanager.h"
#include "history.h"

std::shared_ptr<QSettings> settings();

class MySettings : public QObject {
  Q_OBJECT
  Q_PROPERTY(bool rememberHistory READ rememberHistory WRITE setRememberHistory
                 NOTIFY rememberHistoryChanged)
  Q_PROPERTY(int historyCount READ historyCount NOTIFY historyCountChanged)

 signals:
  void rememberHistoryChanged(bool);
  void historyCountChanged(int);

 public:
  MySettings() = default;
  explicit MySettings(QObject* parent) : QObject(parent) {}

  Q_INVOKABLE [[nodiscard]] bool rememberHistory() const {
    return settings()->value("remember_history", true).toBool();
  }
  Q_INVOKABLE void setRememberHistory(bool h) {
    auto s = settings();
    s->setValue("remember_history", h);
    s->sync();
  }

  Q_INVOKABLE [[nodiscard]] int historyCount() { return history_count(); }

  Q_INVOKABLE void clearHistory() {
    clear_history();
    emit historyCountChanged(historyCount());
  }
};

#endif  // SETTINGS_H
