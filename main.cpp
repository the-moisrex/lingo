#include "dictionarieslistmodel.h"
#include "icons.h"
#include "onlinetranslators.h"
#include "resource_options_model.h"
#include "settings.h"
#include <QDebug>
#include <QFontDatabase>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSslSocket>
#include <QTextToSpeech>
#include <QtQml>

int main(int argc, char *argv[]) {
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

  QGuiApplication app(argc, argv);
  app.setOrganizationName("Lingo");
  app.setApplicationDisplayName("Lingo");
  app.setApplicationName("lingo");
  app.setOrganizationDomain("lingo");
  QFontDatabase::addApplicationFont(":/materialdesignicons-webfont.ttf");

  qDebug() << (QSslSocket::supportsSsl() ? "SSL is supported"
                                         : "SSL is not supported.");

  MySettings settings(nullptr, &app);
  DictionariesListModel dicts;
  QTextToSpeech speech;
  Icons icons;

  dicts.loadDefaults();

  //  qmlRegisterType<DictionariesListModel>("Dictionaries", 1, 0,
  //                                         "DictionariesListModel");
  qmlRegisterUncreatableType<Resource>("Resource", 1, 0, "Resource",
                                       "Resource is not create able.");
  qmlRegisterType<ResourceOptionsModel>("Resource", 1, 0,
                                        "ResourceOptionsModel");
  //  qmlRegisterType<Resource>("Resource", 1, 0, "Resource");
  //  qmlRegisterType<OnlineTranslator<QOnlineTranslator::Google>>(
  //      "Resource.Online.Google", 1, 0, "OnlineTranslatorGoogle");
  //  qmlRegisterType<OnlineTranslator<QOnlineTranslator::Bing>>(
  //      "Resource.Online.Bing", 1, 0, "OnlineTranslatorBing");
  //  qmlRegisterType<OnlineTranslator<QOnlineTranslator::Yandex>>(
  //      "Resource.Online.Yandex", 1, 0, "OnlineTranslatorYandex");

  QQmlApplicationEngine engine;
  auto context = engine.rootContext();
  context->setContextProperty("MySettings", &settings);
  context->setContextProperty("Dictionaries", &dicts);
  context->setContextProperty("Speech", &speech);
  context->setContextProperty("Icons", icons);
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
