#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml>
#include "translator.h"

int main(int argc, char* argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);

  auto translator = new Translator;

  QQmlApplicationEngine engine;
  auto context = engine.rootContext();
  context->setContextProperty("Translator", translator);
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
