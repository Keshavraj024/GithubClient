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

    // Use a fresh query for users
    QSqlQuery userQuery(m_db);
    userQuery.prepare("INSERT OR REPLACE INTO users (id, login, avatar_url, html_url, type) "
                      "VALUES (:id, :login, :avatar, :url, :type)");
    userQuery.bindValue(":id", item.owner.id);
    userQuery.bindValue(":login", item.owner.login);
    userQuery.bindValue(":avatar", item.owner.avatarUrl);
    userQuery.bindValue(":url", item.owner.htmlUrl);
    userQuery.bindValue(":type", item.owner.type);

    if (!userQuery.exec()) {
        qDebug() << "User Error:" << userQuery.lastError().text();
        m_db.rollback();
        return false;
    }

    // Use a fresh query (or clear the old one) for repos
    QSqlQuery repoQuery(m_db);
    repoQuery.prepare(
        "INSERT OR REPLACE INTO saved_repositories "
        "(id, full_name, description, stars, forks, language, html_url, collection_id, owner_id) "
        "VALUES (:id, :name, :desc, :stars, :forks, :lang, :url, :coll, :owner)");

    repoQuery.bindValue(":id", item.id);
    repoQuery.bindValue(":name", item.fullName);
    repoQuery.bindValue(":desc", item.description);
    repoQuery.bindValue(":stars", item.stars);
    repoQuery.bindValue(":forks", item.forks);
    repoQuery.bindValue(":lang", item.language);
    repoQuery.bindValue(":url", item.htmlUrl);
    repoQuery.bindValue(":coll", 0);
    repoQuery.bindValue(":owner", item.owner.id);

    if (!repoQuery.exec()) {
        qDebug() << "Repo Error:" << repoQuery.lastError().text();
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
        item.savedAt = query.value("saved_at").toString();
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

QMap<qlonglong, QString> RepositoryStorage::getAllSavedDates()
{
    QSqlQuery query(m_db);
    query.prepare("SELECT id, saved_at FROM saved_repositories");

    QMap<qlonglong, QString> result;

    if (query.exec()) {
        while (query.next()) {
            // Using index numbers (0, 1) is slightly faster than string names
            qlonglong id = query.value(0).toLongLong();
            QString savedDate = query.value(1).toString();
            result.insert(id, savedDate);
        }
    } else {
        qDebug() << "Database Error:" << query.lastError().text();
    }

    return result;
}
