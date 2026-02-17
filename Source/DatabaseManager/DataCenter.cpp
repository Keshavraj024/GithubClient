#include "DataCenter.h"

DataCenter::DataCenter(QObject *parent)
    : QObject(parent)
{
    // 1. Initialize the DB
    if (!m_dbManager.initialize()) {
        return;
    }

    m_collectionModel = new CollectionModel(m_dbManager.database(), this);
    // m_repoModel = new RepositoryModel(m_dbManager.database(), this);
}

CollectionModel *DataCenter::collections() const
{
    return m_collectionModel;
}
