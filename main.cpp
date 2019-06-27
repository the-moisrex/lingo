#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "dictionarieslistmodel.h"
#include "settings.h"

int main(int argc, char* argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  app.setOrganizationName("Lingo");
  app.setApplicationDisplayName("Lingo");
  app.setApplicationName("lingo");
  app.setOrganizationDomain("lingo");

  DictionariesListModel dicts;
  MySettings settings;

  //  qmlRegisterType<DictionariesListModel>("Dictionaries", 1, 0,
  //                                         "DictionariesListModel");

  QQmlApplicationEngine engine;
  auto context = engine.rootContext();
  context->setContextProperty("MySettings", &settings);
  context->setContextProperty("Dictionaries", &dicts);
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
