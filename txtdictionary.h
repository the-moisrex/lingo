#ifndef TXTDICTIONARY_H
#define TXTDICTIONARY_H

#include "resource.h"
#include <QReadWriteLock>

class txtDictionary : public Resource {
  Q_OBJECT

  struct data_t {
    QList<QString> words;
    QList<QString> translations;
    QString category;
  };

  bool builtin = false;
  QReadWriteLock data_lock;
  QVector<data_t> data;
  QString path;
  QString des =
      tr("Comments format is any line that starts with pound sign. Each "
         "word in a separated line. Alternative words are seperated "
         "with semi-colon. Separator between translation and the word "
         "is a tab character. Examples can be found at: %1")
          .arg("https://www.dicts.info/uddl.php");

  void load(const QString &filepath);
  void setTheOptions();
  void async_search(QString const &word);

public:
  explicit txtDictionary(QObject *parent);
  explicit txtDictionary(QObject *parent, QString builtinpath);

  void setBuiltin(bool value) noexcept { builtin = value; }
  bool isBuiltin() const noexcept { return builtin; }

  QString key() const noexcept override;
  QString description() const noexcept override {
    if (!builtin)
      return des;
    return name();
  }
  ~txtDictionary() noexcept override {}

  bool isSupported(QOnlineTranslator::Language,
                   QOnlineTranslator::Language) const noexcept override;
  virtual bool canProvideSuggestions() const noexcept override { return true; }

  void search(const QString &data) override;

  void componentComplete() override;
};

#endif // TXTDICTIONARY_H
