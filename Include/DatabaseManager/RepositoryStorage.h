#pragma once
#include <QList>
#include <QObject>
#include <QSqlDatabase>
#include "RepositoryItem.h"

class RepositoryStorage
{
public:
    explicit RepositoryStorage(QSqlDatabase db);

    // Creates tables if they don't exist
    bool initSchema();

    // Database Actions
    bool saveRepo(const RepositoryItem &item, const size_t collectionIdx);
    bool removeRepo(qint64 repoId);
    QList<RepositoryItem> loadAll();

    // Check if repo is already saved
    bool exists(qint64 repoId);
    QMap<qlonglong, QString> getAllSavedDates();

private:
    QSqlDatabase m_db;
};
