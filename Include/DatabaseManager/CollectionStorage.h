#pragma once

#include <QDebug>
#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

#include "CollectionItem.h"

class CollectionStorage : public QObject
{
    Q_OBJECT
public:
    explicit CollectionStorage(const QSqlDatabase &db, QObject *parent = nullptr);

    // CRUD Operations
    int addCollection(const QString &name);
    bool removeCollection(int id);
    QList<CollectionItem> fetchAll();
    CollectionItem fetchById(int id);

private:
    QSqlDatabase m_db;
};
