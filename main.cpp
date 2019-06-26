#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVector>
#include <QtQml>
#include "onlinetranslators.h"
#include "resource.h"

QList<Resource> getDictionaries() noexcept {
  QList<Resource> dicts;

  // Default online dictionaries:
  OnlineTranslator<QOnlineTranslator::Google> google;
  OnlineTranslator<QOnlineTranslator::Bing> bing;
  OnlineTranslator<QOnlineTranslator::Yandex> yandex;
  dicts << google << bing << yandex;

  // Default offline dictionaries:

  // Manually added dictionaries:

  return dicts;
}

int main(int argc, char* argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  app.setOrganizationName("Lingo");
  app.setApplicationDisplayName("Lingo");
  app.setApplicationName("lingo");
  app.setOrganizationDomain("lingo");

  auto dicts = getDictionaries();

  //    qmlRegisterType<TranslatorModel>("Translator", 1, 0, "TranslatorModel");

  QQmlApplicationEngine engine;
  auto context = engine.rootContext();
  context->setContextProperty("Dictionaries", dicts);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject* obj, const QUrl& objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
