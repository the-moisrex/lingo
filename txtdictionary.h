#ifndef TXTDICTIONARY_H
#define TXTDICTIONARY_H

#include "resource.h"

class txtDictionary : public Resource {
  Q_OBJECT

  struct data_t {
    QList<QString> names;
    QList<QString> values;
  };

  QVector<data_t> data;
  decltype(data)::const_iterator found = data.cend();

 public:
  explicit txtDictionary(QObject* parent) : Resource(parent) {}
  QString name() const noexcept override { return tr("txt format"); }
  QString key() const noexcept override { return "txt-dic"; }
  QString description() const noexcept override {
    return tr(
        "Plain text format.\n\n"
        "Comments format: line that starts with # sign.\n"
        "Each word in a separated line.\n"
        "Alternative words are seperated with semi-colon.\n"
        "Separator between translation and the word is a tab character.\n"
        "\n"
        "Examples can be found at: https://www.dicts.info/uddl.php");
  }
  ~txtDictionary() noexcept override {}

  bool isSupported(QOnlineTranslator::Language,
                   QOnlineTranslator::Language) const noexcept override;

  void search(const QString& data) override;
};

#endif  // TXTDICTIONARY_H
