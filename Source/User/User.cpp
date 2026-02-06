#include <User.h>

User::User(QObject *parent)
    : QObject(parent)
{}

qint64 User::id() const
{
    return m_id;
}

void User::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString User::login() const
{
    return m_login;
}

void User::setLogin(const QString &newLogin)
{
    if (m_login == newLogin)
        return;
    m_login = newLogin;
    emit loginChanged();
}

QString User::avatarUrl() const
{
    return m_avatarUrl;
}

void User::setAvatarUrl(const QString &newAvatarUrl)
{
    if (m_avatarUrl == newAvatarUrl)
        return;
    m_avatarUrl = newAvatarUrl;
    emit avatarUrlChanged();
}

QString User::htmlUrl() const
{
    return m_htmlUrl;
}

void User::setHtmlUrl(const QString &newHtmlUrl)
{
    if (m_htmlUrl == newHtmlUrl)
        return;
    m_htmlUrl = newHtmlUrl;
    emit htmlUrlChanged();
}

QString User::type() const
{
    return m_type;
}

void User::setType(const QString &newType)
{
    if (m_type == newType)
        return;
    m_type = newType;
    emit typeChanged();
}

User *User::fromJson(const QJsonObject &json, QObject *parent)
{
    auto *user = new User(parent);

    if (json.contains("id"))
        user->setId(json["id"].toVariant().toLongLong());

    if (json.contains("login"))
        user->setLogin(json["login"].toString());

    if (json.contains("avatar_url"))
        user->setAvatarUrl(json["avatar_url"].toString());

    if (json.contains("html_url"))
        user->setHtmlUrl(json["html_url"].toString());

    if (json.contains("type"))
        user->setType(json["type"].toString());

    return user;
}
