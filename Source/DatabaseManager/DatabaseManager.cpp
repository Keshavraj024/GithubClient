#include "DatabaseManager.h"

#include <QDebug>
#include <QDir>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

bool DatabaseManager::initialize()
{
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    QDir().mkpath(dbPath);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", QStringLiteral("github_connection"));

    db.setDatabaseName(dbPath + "/github_client.db");

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    return createTables();
}
QSqlDatabase DatabaseManager::database()
{
    return QSqlDatabase::database(QStringLiteral("github_connection"));
}

bool DatabaseManager::createTables()
{
    QSqlQuery query(database());

    // Collections table
    if (!query.exec("CREATE TABLE IF NOT EXISTS collections ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "name TEXT NOT NULL UNIQUE,"
                    "created_at TEXT DEFAULT CURRENT_TIMESTAMP"
                    ")")) {
        qDebug() << "Create collections table failed:" << query.lastError().text();
        return false;
    }

    // Saved repositories table
    if (!query.exec("CREATE TABLE IF NOT EXISTS saved_repositories ("
                    "id INTEGER PRIMARY KEY," // GitHub repo ID
                    "full_name TEXT,"
                    "description TEXT,"
                    "stars INTEGER,"
                    "forks INTEGER,"
                    "language TEXT,"
                    "html_url TEXT,"
                    "collection_id INTEGER,"
                    "saved_at TEXT DEFAULT CURRENT_TIMESTAMP,"
                    "FOREIGN KEY(collection_id) REFERENCES collections(id)"
                    ")")) {
        qDebug() << "Create saved_repositories table failed:" << query.lastError().text();
        return false;
    }

    return true;
}
