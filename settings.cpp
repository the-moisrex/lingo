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
