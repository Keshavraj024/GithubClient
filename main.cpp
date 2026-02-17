#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include "DataCenter.h"

#include <QQmlContext>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion");
    app.setApplicationName("GitHub Client");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Modern GitHub Client");

    QQmlApplicationEngine engine;

    DataCenter dataCenter;
    engine.rootContext()->setContextProperty("DB", &dataCenter);

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GithubClient", "Main");

    return app.exec();
}
