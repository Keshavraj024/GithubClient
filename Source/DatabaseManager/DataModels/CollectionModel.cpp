#include "CollectionModel.h"
#include <QDebug>
#include <QSqlError>

CollectionModel::CollectionModel(QSqlDatabase db, QObject *parent)
    : QAbstractListModel(parent)
    , m_db(db)
{
    refresh();
}

int CollectionModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_collections.count();
}

QHash<int, QByteArray> CollectionModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[NameRole] = "name";
    roles[CreatedAtRole] = "createdAt";
    return roles;
}

QVariant CollectionModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_collections.count())
        return QVariant();

    const auto &item = m_collections.at(index.row());

    switch (role) {
    case IdRole:
        return item.id;
    case NameRole:
        return item.name;
    case CreatedAtRole:
        return item.createdAt;
    default:
        return QVariant();
    }
}

void CollectionModel::refresh()
{
    beginResetModel();
    m_collections.clear();

    QSqlQuery query(m_db);
    query.prepare("SELECT id, name, created_at FROM collections");

    if (query.exec()) {
        while (query.next()) {
            m_collections.append(
                {query.value(0).toInt(), query.value(1).toString(), query.value(2).toString()});
        }
    } else {
        qWarning() << "Fetch collections failed:" << query.lastError().text();
    }
    endResetModel();
}

void CollectionModel::addCollection(const QString &name)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO collections (name) VALUES (:name)");
    query.bindValue(":name", name);

    if (query.exec()) {
        int newId = query.lastInsertId().toInt();

        QSqlQuery syncQuery(m_db);
        syncQuery.prepare("SELECT id, name, created_at FROM collections WHERE id = :id");
        syncQuery.bindValue(":id", newId);

        if (syncQuery.exec() && syncQuery.next()) {
            beginInsertRows(QModelIndex(), m_collections.count(), m_collections.count());

            m_collections.append({syncQuery.value(0).toInt(),
                                  syncQuery.value(1).toString(),
                                  syncQuery.value(2).toString()});
            endInsertRows();
        }
    } else {
        qWarning() << "Insert failed:" << query.lastError().text();
        // TODO : Here you could emit a signal 'errorOccurred(QString)' to show a popup in QML
    }
}

void CollectionModel::removeCollection(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM collections WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        for (int collectionIdx = 0; collectionIdx < m_collections.count(); ++collectionIdx) {
            if (m_collections[collectionIdx].id == id) {
                beginRemoveRows(QModelIndex(), collectionIdx, collectionIdx);
                m_collections.removeAt(collectionIdx);
                endRemoveRows();
                break;
            }
        }
    }
}
