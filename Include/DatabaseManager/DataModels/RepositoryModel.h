#pragma once

#include <QAbstractListModel>
#include <QSqlDatabase>
#include <QSqlQuery>

struct RepositoryData
{
    int id;
    QString fullName;
    QString description;
    int stars;
    int forks;
    QString language;
    QString htmlUrl;
    int collectionId;
    QString savedAt;
};

class RepositoryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum RepositoryRoles {
        IdRole = Qt::UserRole + 1,
        FullNameRole,
        DescriptionRole,
        StarsRole,
        ForksRole,
        LanguageRole,
        HtmlUrlRole,
        CollectionIdRole,
        SavedAtRole
    };

    explicit RepositoryModel(QSqlDatabase db, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // CRUD API
    Q_INVOKABLE void addRepository(int id,
                                   const QString &fullName,
                                   const QString &desc,
                                   int stars,
                                   int forks,
                                   const QString &lang,
                                   const QString &url,
                                   int colId);
    Q_INVOKABLE void removeRepository(int id);
    Q_INVOKABLE void loadAll();

private:
    QSqlDatabase m_db;
    QList<RepositoryData> m_repos;
};
