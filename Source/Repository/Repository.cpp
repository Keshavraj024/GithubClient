#include "Repository.h"
#include "User.h"
Repository::Repository(QObject *parent)
    : QObject(parent)
{}

qint64 Repository::id() const
{
    return m_id;
}

void Repository::setId(qint64 newId)
{
    if (m_id == newId)
        return;
    m_id = newId;
    emit idChanged();
}

QString Repository::name() const
{
    return m_name;
}

void Repository::setName(const QString &newName)
{
    if (m_name == newName)
        return;
    m_name = newName;
    emit nameChanged();
}

QString Repository::description() const
{
    return m_description;
}

void Repository::setDescription(const QString &newDescription)
{
    if (m_description == newDescription)
        return;
    m_description = newDescription;
    emit descriptionChanged();
}

QString Repository::fullName() const
{
    return m_fullName;
}

void Repository::setFullName(const QString &newFullName)
{
    if (m_fullName == newFullName)
        return;
    m_fullName = newFullName;
    emit fullNameChanged();
}

QString Repository::htmlUrl() const
{
    return m_htmlUrl;
}

void Repository::setHtmlUrl(const QString &newHtmlUrl)
{
    if (m_htmlUrl == newHtmlUrl)
        return;
    m_htmlUrl = newHtmlUrl;
    emit htmlUrlChanged();
}

QString Repository::language() const
{
    return m_language;
}

void Repository::setLanguage(const QString &newLanguage)
{
    if (m_language == newLanguage)
        return;
    m_language = newLanguage;
    emit languageChanged();
}

int Repository::stars() const
{
    return m_stars;
}

void Repository::setStars(int newStars)
{
    if (m_stars == newStars)
        return;
    m_stars = newStars;
    emit starsChanged();
}

int Repository::forks() const
{
    return m_forks;
}

void Repository::setForks(int newForks)
{
    if (m_forks == newForks)
        return;
    m_forks = newForks;
    emit forksChanged();
}

int Repository::openIssues() const
{
    return m_openIssues;
}

void Repository::setOpenIssues(int newOpenIssues)
{
    if (m_openIssues == newOpenIssues)
        return;
    m_openIssues = newOpenIssues;
    emit openIssuesChanged();
}

bool Repository::archived() const
{
    return m_archived;
}

void Repository::setArchived(bool newArchived)
{
    if (m_archived == newArchived)
        return;
    m_archived = newArchived;
    emit archivedChanged();
}

bool Repository::isPrivate() const
{
    return m_isPrivate;
}

void Repository::setIsPrivate(bool newIsPrivate)
{
    if (m_isPrivate == newIsPrivate)
        return;
    m_isPrivate = newIsPrivate;
    emit isPrivateChanged();
}

QDateTime Repository::updatedAt() const
{
    return m_updatedAt;
}

void Repository::setUpdatedAt(const QDateTime &newUpdatedAt)
{
    if (m_updatedAt == newUpdatedAt)
        return;
    m_updatedAt = newUpdatedAt;
    emit updatedAtChanged();
}

QObject *Repository::owner() const
{
    return m_owner;
}

void Repository::setOwner(QObject *newOwner)
{
    if (m_owner == newOwner)
        return;
    m_owner = newOwner;
    emit ownerChanged();
}

Repository *Repository::fromJson(const QJsonObject &json, QObject *parent)
{
    auto *repo = new Repository(parent);

    if (json.contains("id"))
        repo->setId(json["id"].toVariant().toLongLong());

    if (json.contains("name"))
        repo->setName(json["name"].toString());

    if (json.contains("full_name"))
        repo->setFullName(json["full_name"].toString());

    if (json.contains("description") && !json["description"].isNull())
        repo->setDescription(json["description"].toString());

    if (json.contains("html_url"))
        repo->setHtmlUrl(json["html_url"].toString());

    if (json.contains("language") && !json["language"].isNull())
        repo->setLanguage(json["language"].toString());

    // ---- Counts
    if (json.contains("stargazers_count"))
        repo->setStars(json["stargazers_count"].toInt());

    if (json.contains("forks_count"))
        repo->setForks(json["forks_count"].toInt());

    if (json.contains("open_issues_count"))
        repo->setOpenIssues(json["open_issues_count"].toInt());

    // ---- Flags
    if (json.contains("archived"))
        repo->setArchived(json["archived"].toBool());

    if (json.contains("private"))
        repo->setIsPrivate(json["private"].toBool());

    // ---- Date handling (ISO 8601)
    if (json.contains("updated_at")) {
        const QString updatedAtStr = json["updated_at"].toString();
        if (!updatedAtStr.isEmpty()) {
            const QDateTime updatedAt = QDateTime::fromString(updatedAtStr, Qt::ISODate);
            if (updatedAt.isValid())
                repo->setUpdatedAt(updatedAt);
        }
    }

    // ---- Owner
    if (json.contains("owner") && json["owner"].isObject()) {
        const QJsonObject userObject = json["owner"].toObject();
        if (!userObject.isEmpty()) {
            if (auto *user = User::fromJson(userObject, repo)) {
                repo->setOwner(user);
            }
        }
    }

    return repo;
}
