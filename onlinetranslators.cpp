#include "onlinetranslators.h"
#include <QDebug>
#include <QOnlineTranslator>

QString formatData(const QOnlineTranslator& translator) {
  std::cout << translator.translation().toStdString() << std::endl;

  QString tr;
  // Translation
  tr = translator.translation().toHtmlEscaped().replace("\n", "<br>");

  // Translit
  if (!translator.translationTranslit().isEmpty())
    tr.append(" <font color=\"grey\"><i>/" +
              translator.translationTranslit().replace("\n", "/<br>/") +
              "/</i></font> ");
  if (!translator.sourceTranslit().isEmpty())
    tr.append(" <font color=\"grey\"><i><b>(" +
              translator.sourceTranslit().replace("\n", "/<br>/") +
              ")</b></i></font> ");

  // Transcription
  if (!translator.sourceTranscription().isEmpty())
    tr.append(" <font color=\"grey\">[" + translator.sourceTranscription() +
              "]</font> ");

  tr += "<br>";

  // Translation options
  if (!translator.translationOptions().isEmpty()) {
    tr.append(" <font color=\"grey\"><i>" + translator.source() + "</i> – " +
              QObject::tr("translation options:") + "</font> ");

    // Print words for each type of speech
    foreach (const QOption& option, translator.translationOptions()) {
      tr.append(" <b>" + option.typeOfSpeech() + "</b> ");
      // TODO: indent

      for (int i = 0; i < option.count(); ++i) {
        // Show word gender
        if (!option.gender(i).isEmpty())
          tr.append(" <i>" + option.gender(i) + "</i> ");

        // Show Word
        tr.append(option.word(i));

        // Show word meaning
        if (!option.translations(i).isEmpty()) {
          tr.append(": ");
          tr.append(" <font color=\"grey\"><i>");
          tr.append(option.translations(i));
          tr.append("</i></font> ");
        }
      }

      tr += "<br>";  // Add a new line before the next type of speech
    }
  }

  // Examples
  if (!translator.examples().isEmpty()) {
    tr.append(" <font color=\"grey\"><i>" + translator.source() + "</i> – " +
              QObject::tr("examples:") + "</font> ");
    foreach (const QExample& example, translator.examples()) {
      tr.append(" <b>" + example.typeOfSpeech() + "</b> ");
      for (int i = 0; i < example.count(); ++i) {
        tr.append(example.description(i));
        tr.append(" <font color=\"grey\"><i>" + example.example(i) +
                  "</i></font> ");
        tr.append("<br>");
      }
    }
  }

  //  qDebug() << tr;
  return tr;
}

template <QOnlineTranslator::Engine Engine>
void OnlineTranslator<Engine>::search(const QString& data) {
  // loading changes so the ui gets affected
  setLoading(true);
  translator->translate(data, Engine, to, from);
}
