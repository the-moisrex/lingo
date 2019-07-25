#ifndef TXTDICTIONARY_H
#define TXTDICTIONARY_H

#include "resource.h"

class txtDictionary : public Resource {
  Q_OBJECT

  struct data_t {
    QList<QString> words;
    QList<QString> translations;
    QString category;
  };

  bool builtin = false;
  QVector<data_t> data;
  QString path;

  void load(const QString& filepath);
  void setTheOptions();

 public:
  explicit txtDictionary(QObject* parent);
  explicit txtDictionary(QObject* parent, QString builtinpath);

  void setBuiltin(bool value) noexcept { builtin = value; }
  bool isBuiltin() const noexcept { return builtin; }

  QString key() const noexcept override;
  QString description() const noexcept override;
  ~txtDictionary() noexcept override {}

  bool isSupported(QOnlineTranslator::Language,
                   QOnlineTranslator::Language) const noexcept override;
  virtual bool canProvideSuggestions() const noexcept override { return true; }

  void search(const QString& data) override;

  void componentComplete() override;
};

#endif  // TXTDICTIONARY_H
