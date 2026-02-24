#pragma once

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "CollectionItem.h"

class CollectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CollectionRoles { IdRole = Qt::UserRole + 1, NameRole, CreatedAtRole };

    explicit CollectionModel(QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // CRUD API for QML
    void setCollections(const QList<CollectionItem> &collections);
    void appendCollection(const CollectionItem &item);
    void removeCollectionFromModel(int id);

private:
    QList<CollectionItem> m_collections;
};
