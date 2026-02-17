#pragma once

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>

struct CollectionData
{
    int id;
    QString name;
    QString createdAt;
};

class CollectionModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum CollectionRoles { IdRole = Qt::UserRole + 1, NameRole, CreatedAtRole };

    explicit CollectionModel(QSqlDatabase db, QObject *parent = nullptr);

    // QAbstractListModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // CRUD API for QML
    Q_INVOKABLE void addCollection(const QString &name);
    Q_INVOKABLE void removeCollection(int id);
    Q_INVOKABLE void refresh(); // Reloads from SQL

private:
    QSqlDatabase m_db;
    QList<CollectionData> m_collections;
};
