#include "CollectionModel.h"
#include <QDebug>
#include <QSqlError>

CollectionModel::CollectionModel(QObject *parent)
    : QAbstractListModel(parent)
{
    
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

void CollectionModel::setCollections(const QList<CollectionItem> &collections)
{
    beginResetModel();
    m_collections = collections;
    endResetModel();
}

void CollectionModel::appendCollection(const CollectionItem &item)
{
    beginInsertRows(QModelIndex(), m_collections.count(), m_collections.count());
    m_collections.append(item);
    endInsertRows();
}

void CollectionModel::removeCollectionFromModel(int id)
{
    for (int i = 0; i < m_collections.count(); ++i) {
        if (m_collections[i].id == id) {
            beginRemoveRows(QModelIndex(), i, i);
            m_collections.removeAt(i);
            endRemoveRows();
            break;
        }
    }
}
