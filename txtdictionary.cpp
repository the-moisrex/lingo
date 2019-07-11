#include "txtdictionary.h"
#include <QCryptographicHash>
#include <QTextStream>
#include <algorithm>

void txtDictionary::load(const QString filepath) {
  QFile file(filepath);
  if (file.exists() && file.open(QIODevice::ReadOnly)) {
    setInitStatus(true);
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine().trimmed();

      // it's a comment
      if (line[0] == '#')
        continue;

      auto parts = line.split("\t");
      auto words = parts[0].split(";");
      auto translations = parts[1].split(";");
      auto category = parts[2].trimmed();

      // remove unneeded spaces:
      std::transform(words.begin(), words.end(), words.begin(),
                     [](auto& word) { return word.trimmed(); });
      std::transform(translations.begin(), translations.end(),
                     translations.begin(),
                     [](auto& translation) { return translation.trimmed(); });

      data.push_back(data_t{words, translations, category});
    }
    file.close();
    setInitStatus(false);
  } else {
    setEnabled(false);
  }
}

void txtDictionary::setTheOptions() {
  if (path == "" && optionValue("filepath").toString() != "")
    setOption({resource_option::TEXT, "filepath", "", "File Path"});
}

txtDictionary::txtDictionary(QObject* parent) : Resource(parent) {
  setName("TXT Format");
}

txtDictionary::txtDictionary(QObject* parent, QString builtinpath)
    : Resource(parent), builtin(true), path(std::move(builtinpath)) {
  setId(QString(QCryptographicHash::hash(path.toStdString().c_str(),
                                         QCryptographicHash::Md5)
                    .toHex()));
}

QString txtDictionary::key() const noexcept {
  return "txt-dic";
}

QString txtDictionary::description() const noexcept {
  if (!builtin)
    return tr(
        "Plain text format.\n\n"
        "Comments format: line that starts with # sign.\n"
        "Each word in a separated line.\n"
        "Alternative words are seperated with semi-colon.\n"
        "Separator between translation and the word is a tab character.\n"
        "\n"
        "Examples can be found at: https://www.dicts.info/uddl.php");
  return name();
}

bool txtDictionary::isSupported(QOnlineTranslator::Language,
                                QOnlineTranslator::Language) const noexcept {
  return true;  // TODO: chagne this
}

void txtDictionary::search(const QString& word) {
  setLoading(true);

  found = std::find_if(data.cbegin(), data.cend(), [&](auto const& a) {
    return a.words.end() != std::find(a.words.begin(), a.words.end(), word);
  });

  setLoading(false);
}

void txtDictionary::componentComplete() {
  Resource::componentComplete();
  setTheOptions();
  if (!builtin)
    path = optionValue("path", "").toString();
  if (path != "")
    load(path);
}
