#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "DatabaseManager.h"
#include "RepositoryItem.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Fusion");
    app.setApplicationName("GitHub Client");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("Modern GitHub Client");

    QQmlApplicationEngine engine;

    DatabaseManager db;
    if (!db.initialize()) {
        qWarning() << "Database init failed";
    }

    qRegisterMetaType<RepositoryItem>("RepositoryItem");
    qRegisterMetaType<QList<RepositoryItem>>("QList<RepositoryItem>");

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("GithubClient", "Main");

    return app.exec();
}
