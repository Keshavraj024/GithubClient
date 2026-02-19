#include "RepositoryStorage.h"
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

RepositoryStorage::RepositoryStorage(QSqlDatabase db)
    : m_db(db)
{}

bool RepositoryStorage::saveRepo(const RepositoryItem &item)
{
    if (!m_db.transaction())
        return false;

    QSqlQuery query(m_db);

    // Save User first
    query.prepare("INSERT OR REPLACE INTO users (id, login, avatar_url, html_url, type) "
                  "VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(item.owner.id);
    query.addBindValue(item.owner.login);
    query.addBindValue(item.owner.avatarUrl);
    query.addBindValue(item.owner.htmlUrl);
    query.addBindValue(item.owner.type);

    if (!query.exec()) {
        m_db.rollback();
        return false;
    }

    // Save Repo referencing User
    query.prepare("INSERT OR REPLACE INTO saved_repositories "
                  "(id, full_name, description, stars, forks, language, html_url, owner_id) "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(item.id);
    query.addBindValue(item.fullName);
    query.addBindValue(item.description);
    query.addBindValue(item.stars);
    query.addBindValue(item.forks);
    query.addBindValue(item.language);
    query.addBindValue(item.htmlUrl);
    query.addBindValue(item.owner.id);

    if (!query.exec()) {
        m_db.rollback();
        return false;
    }

    return m_db.commit();
}

QList<RepositoryItem> RepositoryStorage::loadAll()
{
    QList<RepositoryItem> list;
    // We use a JOIN to get User and Repo info in one go
    QSqlQuery
        query("SELECT repo.*, user.login, user.avatar_url, user.html_url as user_url, user.type "
              "FROM saved_repositories repo "
              "JOIN users user ON repo.owner_id = user.id",
              m_db);

    while (query.next()) {
        RepositoryItem item;
        item.id = query.value("id").toLongLong();
        item.fullName = query.value("full_name").toString();
        item.description = query.value("description").toString();
        item.stars = query.value("stars").toInt();
        item.forks = query.value("forks").toInt();
        item.language = query.value("language").toString();
        item.htmlUrl = query.value("html_url").toString();
        item.isLocallySaved = true;

        item.owner.id = query.value("owner_id").toLongLong();
        item.owner.login = query.value("login").toString();
        item.owner.avatarUrl = query.value("avatar_url").toString();
        item.owner.htmlUrl = query.value("user_url").toString();
        item.owner.type = query.value("type").toString();

        list.append(item);
    }
    return list;
}

bool RepositoryStorage::removeRepo(qint64 repoId)
{
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM saved_repositories WHERE id = ?");
    query.addBindValue(repoId);
    return query.exec();
}

bool RepositoryStorage::exists(qint64 repoId)
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM saved_repositories WHERE id = ?");
    query.addBindValue(repoId);
    return query.exec() && query.next();
}
