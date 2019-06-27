#ifndef TXTDICTIONARY_H
#define TXTDICTIONARY_H

#include "resource.h"

class txtDictionary : public Resource {
 public:
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

  void search(const QString& data) override;
};

#endif  // TXTDICTIONARY_H
