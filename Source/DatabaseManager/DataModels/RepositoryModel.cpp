#include "RepositoryModel.h"
#include <algorithm>

RepositoryModel::RepositoryModel(QObject *parent)
    : QAbstractListModel(parent)
{}

int RepositoryModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_repos.size();
}

QHash<int, QByteArray> RepositoryModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "repoId";
    roles[NameRole] = "name";
    roles[FullNameRole] = "fullName";
    roles[DescriptionRole] = "description";
    roles[HtmlUrlRole] = "htmlUrl";
    roles[LanguageRole] = "language";
    roles[StarsRole] = "stars";
    roles[ForksRole] = "forks";
    roles[OpenIssuesRole] = "openIssues";
    roles[ArchivedRole] = "archived";
    roles[IsPrivateRole] = "isPrivate";
    roles[UpdatedAtRole] = "updatedAt";
    roles[OwnerIdRole] = "ownerId";
    roles[OwnerLoginRole] = "ownerLogin";
    roles[OwnerAvatarUrlRole] = "ownerAvatarUrl";
    roles[OwnerHtmlUrlRole] = "ownerHtmlUrl";
    roles[OwnerTypeRole] = "ownerType";
    roles[CollectionIdRole] = "collectionId";
    roles[SavedAtRole] = "savedAt";
    roles[IsSavedRole] = "isSaved";
    return roles;
}

QVariant RepositoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_repos.size())
        return QVariant();

    const auto &item = m_repos[index.row()];
    switch (role) {
    case IdRole:
        return item.id;
    case NameRole:
        return item.name;
    case FullNameRole:
        return item.fullName;
    case DescriptionRole:
        return item.description;
    case HtmlUrlRole:
        return item.htmlUrl;
    case LanguageRole:
        return item.language;
    case StarsRole:
        return item.stars;
    case ForksRole:
        return item.forks;
    case OpenIssuesRole:
        return item.openIssues;
    case ArchivedRole:
        return item.archived;
    case IsPrivateRole:
        return item.isPrivate;
    case UpdatedAtRole:
        return item.updatedAt;
    case OwnerIdRole:
        return item.owner.id;
    case OwnerLoginRole:
        return item.owner.login;
    case OwnerAvatarUrlRole:
        return item.owner.avatarUrl;
    case OwnerHtmlUrlRole:
        return item.owner.htmlUrl;
    case OwnerTypeRole:
        return item.owner.type;
    case CollectionIdRole:
        return item.collectionId;
    case SavedAtRole:
        return item.savedAt;
    case IsSavedRole:
        return item.isLocallySaved;
    }
    return QVariant();
}

void RepositoryModel::updateFromApi(const QList<RepositoryItem> &apiResults)
{
    for (const auto &newItem : apiResults) {
        auto it = std::find_if(m_repos.begin(), m_repos.end(), [&](const RepositoryItem &item) {
            return item.id == newItem.id;
        });

        if (it != m_repos.end()) {
            it->stars = newItem.stars;
            it->forks = newItem.forks;
            it->openIssues = newItem.openIssues;
            it->updatedAt = newItem.updatedAt;
            int row = std::distance(m_repos.begin(), it);
            emit dataChanged(index(row), index(row));
        } else {
            beginInsertRows(QModelIndex(), m_repos.size(), m_repos.size());
            m_repos.append(newItem);
            endInsertRows();
        }
    }
}

void RepositoryModel::loadFromDatabase(const QList<RepositoryItem> &dbResults)
{
    beginResetModel();
    m_repos = dbResults;
    for (auto &item : m_repos)
        item.isLocallySaved = true;
    endResetModel();
}

void RepositoryModel::clearSearch()
{
    qDebug() << m_repos.size();
    for (int i = m_repos.size() - 1; i >= 0; --i) {
        if (!m_repos[i].isLocallySaved) {
            beginRemoveRows(QModelIndex(), i, i);
            m_repos.removeAt(i);
            endRemoveRows();
        }
    }
    qDebug() << "remove";
}

RepositoryItem &RepositoryModel::getItem(int row)
{
    return m_repos[row];
}

void RepositoryModel::notifyRowChanged(int row)
{
    QModelIndex idx = index(row);
    emit dataChanged(idx, idx);
}
