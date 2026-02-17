#pragma once

#include <QObject>
#include "CollectionModel.h"
#include "DatabaseManager.h"

class DataCenter : public QObject
{
    Q_OBJECT
    // Expose your models here
    Q_PROPERTY(CollectionModel *collections READ collections CONSTANT)
    // Q_PROPERTY(RepositoryModel* repositories READ repositories CONSTANT)

public:
    explicit DataCenter(QObject *parent = nullptr);

    CollectionModel *collections() const;
    // RepositoryModel* repositories() const { return m_repoModel; }

private:
    DatabaseManager m_dbManager;
    CollectionModel *m_collectionModel;
    // RepositoryModel* m_repoModel;
};
