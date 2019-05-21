#ifndef WORD_H
#define WORD_H

#include <QString>
#include <QVector>
#include <QtAlgorithms>

struct Word {
  QString value;
  QString langCode;
};

class Dictionary : public QVector<Word> {
  Word const& getInLangCode(QString const& langCode) const {
    for (auto const& word : *this)
      if (word.langCode == langCode)
        return word;
    throw std::out_of_range(langCode.toStdString().c_str());
  }
};

#endif  // WORD_H
