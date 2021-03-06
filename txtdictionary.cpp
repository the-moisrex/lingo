#include "txtdictionary.h"
#include <QCryptographicHash>
#include <QDebug>
#include <QReadLocker>
#include <QTextStream>
#include <QWriteLocker>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>

void txtDictionary::load(const QString &filepath) {
  //  qDebug() << "Starting loading " << filepath;

  QFile file(filepath);
  if (file.exists() && file.open(QIODevice::ReadOnly | QFile::Text)) {
    setInitStatus(true);
    //    qDebug() << "Start reading file " << filepath;
    QTextStream in(&file);
    {
      QWriteLocker locked(&data_lock);
      while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        //        qDebug() << line;

        // it's a comment
        if (!line.size() || line[0] == '#')
          continue;

        auto parts = line.split("\t");
        if (parts.size() <= 2)
          continue;
        auto words = parts[0].split(";");
        auto translations = parts[1].split(";");
        auto category = parts.size() > 2 ? parts[2].trimmed() : "";

        // remove unneeded spaces and convert to lowercase:
        std::transform(words.begin(), words.end(), words.begin(),
                       [](auto &word) { return word.toLower().trimmed(); });
        std::transform(translations.begin(), translations.end(),
                       translations.begin(),
                       [](auto &translation) { return translation.trimmed(); });

        data.push_back(data_t{words, translations, category});
      }
    }
    file.close();
    //    qDebug() << "Loading of " << filepath << "is done";
    setTempEnabled(true);
    setInitStatus(false);
  } else {
    qDebug() << "Couldn't load file " << filepath;
    setTempEnabled(false);
  }
}

void txtDictionary::setTheOptions() {
  if (!builtin) {
    setOptionIfNotExists(
        {resource_option::TEXT, "filepath", QVariant(), tr("File Path")});
    setOptionIfNotExists({resource_option::LANGUAGE_SELECTOR, "from-lang",
                          QOnlineTranslator::Language::Auto,
                          tr("From Language")});
    setOptionIfNotExists({resource_option::LANGUAGE_SELECTOR, "to-lang",
                          QOnlineTranslator::Language::Spanish,
                          tr("To Language")});
  }
}

void txtDictionary::async_search(const QString &word) {
  setLoading(true);
  {
    QReadLocker locked(&data_lock);
    auto found = std::find_if(data.cbegin(), data.cend(), [&](auto const &a) {
      return a.words.end() != std::find(a.words.begin(), a.words.end(), word);
    });

    if (found != data.cend()) {
      QString t = "";
      for (auto const &trans : found->translations) {
        t.append(trans);
        t.append("<br>");
      }
      if (found->category.size()) {
        t.append(QObject::tr("Category: "));
        t.append("<b>");
        t.append(found->category);
        t.append("</b>");
      }

      setTranslation(t);
      //      qDebug() << "Translation found for word '" << word
      //               << "' in txtDictionary";
    } else {
      clearTranslation();
      qDebug() << "Search for word '" << word << "' found no results.";
    }
  }

  setLoading(false);
}

txtDictionary::txtDictionary(QObject *parent) : Resource(parent) {}

txtDictionary::txtDictionary(QObject *parent, QString builtinpath)
    : Resource(parent), builtin(true), path(std::move(builtinpath)) {
  // setId(QString(QCryptographicHash::hash(path.toStdString().c_str(),
  //                                        QCryptographicHash::Md5)
  //                   .toHex()));
}

QString txtDictionary::key() const noexcept { return "txt-dic"; }

bool txtDictionary::isSupported(QOnlineTranslator::Language _from,
                                QOnlineTranslator::Language _to) const
    noexcept {
  if (builtin) {
    if (_from == QOnlineTranslator::Language::English ||
        _from == QOnlineTranslator::Auto)
      if (_to == QOnlineTranslator::Language::Spanish)
        return true;
    return false;
  }
  auto from = static_cast<QOnlineTranslator::Language>(
      optionValue("from-lang").toInt());
  auto to =
      static_cast<QOnlineTranslator::Language>(optionValue("to-lang").toInt());
  if (from == QOnlineTranslator::Auto || from == _from) {
    if (to == _to)
      return true;
  }
  return false;
}

void txtDictionary::search(const QString &word) {
  auto _word = word.toLower().trimmed();

  //  qDebug() << "Searching for word '" << word << "' in txtDictionary ("
  //           << data.size() << " words)";

  QtConcurrent::run(this, &txtDictionary::async_search, _word);
}

void txtDictionary::componentComplete() {
  Resource::componentComplete();
  setTheOptions();
  if (!builtin)
    path = optionValue("filepath", "").toString();
  if (path != "") {
    QtConcurrent::run(this, &txtDictionary::load, path);
  } else {
    setTempEnabled(false);
    setInitStatus(false);
  }
}
