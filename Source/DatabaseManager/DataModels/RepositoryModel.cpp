#include "RepositoryModel.h"
#include <QDebug>
#include <QSqlError>

RepositoryModel::RepositoryModel(QSqlDatabase db, QObject *parent)
    : QAbstractListModel(parent)
    , m_db(db)
{
    loadAll();
}

int RepositoryModel::rowCount(const QModelIndex &parent) const
{
    return m_repos.count();
}

QHash<int, QByteArray> RepositoryModel::roleNames() const
{
    return {{IdRole, "repoId"},
            {FullNameRole, "fullName"},
            {DescriptionRole, "description"},
            {StarsRole, "stars"},
            {ForksRole, "forks"},
            {LanguageRole, "language"},
            {HtmlUrlRole, "htmlUrl"},
            {CollectionIdRole, "collectionId"},
            {SavedAtRole, "savedAt"}};
}

QVariant RepositoryModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_repos.count())
        return QVariant();
    const auto &r = m_repos.at(index.row());

    switch (role) {
    case IdRole:
        return r.id;
    case FullNameRole:
        return r.fullName;
    case DescriptionRole:
        return r.description;
    case StarsRole:
        return r.stars;
    case ForksRole:
        return r.forks;
    case LanguageRole:
        return r.language;
    case HtmlUrlRole:
        return r.htmlUrl;
    case CollectionIdRole:
        return r.collectionId;
    case SavedAtRole:
        return r.savedAt;
    }
    return QVariant();
}

void RepositoryModel::addRepository(int id,
                                    const QString &fullName,
                                    const QString &desc,
                                    int stars,
                                    int forks,
                                    const QString &lang,
                                    const QString &url,
                                    int colId)
{
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO saved_repositories (id, full_name, description, stars, forks, "
                  "language, html_url, collection_id) "
                  "VALUES (:id, :name, :desc, :stars, :forks, :lang, :url, :colId)");
    query.bindValue(":id", id);
    query.bindValue(":name", fullName);
    query.bindValue(":desc", desc);
    query.bindValue(":stars", stars);
    query.bindValue(":forks", forks);
    query.bindValue(":lang", lang);
    query.bindValue(":url", url);
    query.bindValue(":colId", colId);

    if (query.exec()) {
        QSqlQuery sync(m_db);
        sync.prepare("SELECT saved_at FROM saved_repositories WHERE id = :id");
        sync.bindValue(":id", id);

        if (sync.exec() && sync.next()) {
            beginInsertRows(QModelIndex(), m_repos.count(), m_repos.count());
            m_repos.append(
                {id, fullName, desc, stars, forks, lang, url, colId, sync.value(0).toString()});
            endInsertRows();
        }
    }
}

void RepositoryModel::removeRepository(int id)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM saved_repositories WHERE id = :id");
    query.bindValue(":id", id);

    if (query.exec()) {
        for (int repoIdx = 0; repoIdx < m_repos.count(); ++repoIdx) {
            if (m_repos[repoIdx].id == id) {
                beginRemoveRows(QModelIndex(), repoIdx, repoIdx);
                m_repos.removeAt(repoIdx);
                endRemoveRows();
                break;
            }
        }
    }
}

void RepositoryModel::loadAll()
{
    beginResetModel();
    m_repos.clear();
    QSqlQuery query("SELECT * FROM saved_repositories", m_db);
    while (query.next()) {
        m_repos.append({query.value("id").toInt(),
                        query.value("full_name").toString(),
                        query.value("description").toString(),
                        query.value("stars").toInt(),
                        query.value("forks").toInt(),
                        query.value("language").toString(),
                        query.value("html_url").toString(),
                        query.value("collection_id").toInt(),
                        query.value("saved_at").toString()});
    }
    endResetModel();
}
