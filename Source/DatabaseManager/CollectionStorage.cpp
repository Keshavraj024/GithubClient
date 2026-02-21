#include "CollectionStorage.h"

CollectionStorage::CollectionStorage(const QSqlDatabase &db, QObject *parent)
    : QObject(parent)
    , m_db(db)
{}

int CollectionStorage::addCollection(const QString &name)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO collections (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (!query.exec()) {
        qWarning() << "Database Error (Add Collection):" << query.lastError().text();
        return -1;
    }

    return query.lastInsertId().toInt();
}

bool CollectionStorage::removeCollection(int id)
{
    // Note: If you have foreign keys enabled, this might affect 'saved_repositories'
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM collections WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Database Error (Remove Collection):" << query.lastError().text();
        return false;
    }

    return true;
}

QList<CollectionItem> CollectionStorage::fetchAll()
{
    QList<CollectionItem> list;
    QSqlQuery query("SELECT id, name, created_at FROM collections", m_db);

    while (query.next()) {
        list.append({query.value(0).toInt(), query.value(1).toString(), query.value(2).toString()});
    }
    return list;
}

CollectionItem CollectionStorage::fetchById(int id)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, created_at FROM collections WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return {query.value(0).toInt(), query.value(1).toString(), query.value(2).toString()};
    }
    return {};
}
