#include "settings.h"
#include <QStandardPaths>

std::shared_ptr<QSettings> settings() {
  static std::shared_ptr<QSettings> config;
  if (config)
    return config;

  QString filename = "lingo.settings.ini";
  QString path =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  config =
      std::make_shared<QSettings>(filename + "/" + path, QSettings::IniFormat);

  return config;
}

void MySettings::useUILanguage(int index) {
  switch (index) {
  case 0:
    break;
  case 1:
    uitranslator.load(":/langs/lingo-fa.qm");
    break;
  }
  if (index != 0) {
    app->installTranslator(&uitranslator);
  }
}

QStringList MySettings::getUILanguages() const {
  return QStringList() << "English"
                       << "فارسی";
}

int MySettings::getUILanguage() const {
  return settings()->value("ui-lang", 0).toInt();
}

void MySettings::setUILanguage(int index) {
  auto _settings = settings();
  _settings->setValue("ui-lang", index);
  _settings->sync();
  useUILanguage(index);
}

bool MySettings::isRTL() const { return getUILanguage() == 1; }
