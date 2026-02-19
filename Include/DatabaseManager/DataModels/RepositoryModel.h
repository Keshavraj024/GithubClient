#pragma once

#include <QAbstractListModel>
#include <QByteArray>
#include <QHash>
#include "RepositoryItem.h"

class RepositoryModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        FullNameRole,
        DescriptionRole,
        HtmlUrlRole,
        LanguageRole,
        StarsRole,
        ForksRole,
        OpenIssuesRole,
        ArchivedRole,
        IsPrivateRole,
        UpdatedAtRole,
        OwnerIdRole,
        OwnerLoginRole,
        OwnerAvatarUrlRole,
        OwnerHtmlUrlRole,
        OwnerTypeRole,
        CollectionIdRole,
        SavedAtRole,
        IsSavedRole
    };

    explicit RepositoryModel(QObject *parent = nullptr);

    // QAbstractItemModel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    // Data Management
    void updateFromApi(const QList<RepositoryItem> &apiResults);
    void loadFromDatabase(const QList<RepositoryItem> &dbResults);
    void clearSearch();
    RepositoryItem &getItem(int row);
    void notifyRowChanged(int row);

private:
    QList<RepositoryItem> m_repos;
};
