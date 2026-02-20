#include "GithubService.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

GitHubService::GitHubService(QObject *parent)
    : QObject{parent}
    , m_networkManager(new QNetworkAccessManager(this))

{
    // fetchTrendingRepositories();
}

RepositoryItem GitHubService::parseRepositoryJson(const QJsonObject &json)
{
    RepositoryItem repo;
    // Direct mapping to struct members
    repo.id = json["id"].toVariant().toLongLong();
    repo.name = json["name"].toString();
    repo.fullName = json["full_name"].toString();
    repo.stars = json["stargazers_count"].toInt();
    repo.forks = json["forks_count"].toInt();
    repo.isPrivate = json["private"].toBool();
    repo.htmlUrl = json["html_url"].toString();
    repo.openIssues = json["open_issues_count"].toInt();
    repo.archived = json["archived"].toBool();
    repo.language = json["language"].toString();

    if (!json["description"].isNull())
        repo.description = json["description"].toString();

    QDateTime updated = QDateTime::fromString(json["updated_at"].toString(), Qt::ISODate);
    if (updated.isValid())
        repo.updatedAt = updated;

    // Mapping Nested User Information
    if (json.contains("owner") && json["owner"].isObject()) {
        QJsonObject ownerJson = json["owner"].toObject();

        repo.owner.id = ownerJson["id"].toVariant().toLongLong(); // Match qint64
        repo.owner.login = ownerJson["login"].toString();
        repo.owner.avatarUrl = ownerJson["avatar_url"].toString();
        repo.owner.htmlUrl = ownerJson["html_url"].toString();
        repo.owner.type = ownerJson["type"].toString();
    } else {
        qWarning() << "Owner data missing for repo:" << repo.fullName;
    }

    return repo;
}

void GitHubService::fetchUserRepositories(const QString &username)
{
    qDebug() << "Fetching User Repo 01";
    QString endpoint = QString("users/%1/repos").arg(username);
    QUrl url(QString("https://api.github.com/%1?sort=updated&per_page=50").arg(endpoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    // if (!authToken.isEmpty()) {
    //     request.setRawHeader("Authorization", QString("Bearer %1").arg(authToken).toUtf8());
    // }

    qDebug() << "Fetching User Repo 01- 01";
    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "userRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService::onUserRepositoriesFetched);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::requestFailed);
}

// void GitHubService::fetchRepository(const QString &user, const QString &repo)
// {
//     if (m_isLoading || user.isEmpty() || repo.isEmpty()) {
//         return;
//     }

//     setIsLoading(true);
//     setErrorMessage(QString());
//     clearRepositories();

//     QUrl url(QString("https://api.github.com/repos/%1/%2").arg(user, repo));
//     QNetworkRequest request(url);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//     request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
//     request.setRawHeader("Accept", "application/vnd.github.v3+json");

//     if (!m_authToken.isEmpty()) {
//         request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
//     }

//     QNetworkReply *reply = m_networkManager->get(request);
//     reply->setProperty("requestType", "userSingleRepository");

//     connect(reply, &QNetworkReply::finished, this, &GitHubService ::onUserRepositoryReceived);
//     connect(reply,
//             QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
//             this,
//             &GitHubService::onRequestFailed);
// }

// void GitHubService::clearRepositories()
// {
//     if (!m_repositories.isEmpty()) {
//         m_repositories.clear();
//         emit repositoriesChanged();
//     }
// }

void GitHubService::fetchRemoteRepositories(const QString &query,
                                            const QString &sort,
                                            const QString &order)
{
    qDebug() << "Fetching Remote Repo 01";
    QUrl url(QString("https://api.github.com/search/repositories?q=%1&sort=%2&order=%3&per_page=50")
                 .arg(query, sort, order));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    // // TODO : if this field is not empty then it throws error
    // if (!m_authToken.isEmpty()) {
    //     request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    // }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "searchRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onRemoteRepositoriesFetched);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::requestFailed);
}

void GitHubService::fetchAuthenticatedUserRepositories(const QString &authToken)
{
    QUrl url(QString("https://api.github.com/user/repos?sort=updated&per_page=50&visibility=all"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(authToken).toUtf8());

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "userRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onUserRepositoriesFetched);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::requestFailed);
}

// void GitHubService::fetchTrendingRepositories(const int days)
// {
//     if (m_isLoading) {
//         return;
//     }

//     setIsLoading(true);
//     setErrorMessage(QString());
//     clearRepositories();

//     QDate date = QDate::currentDate().addDays(-days);
//     QString query = QString("created:>%1").arg(date.toString("yyyy-MM-dd"));

//     QUrl url(
//         QString("https://api.github.com/search/repositories?q=%1&sort=stars&order=desc&per_page=50")
//             .arg(query));
//     QNetworkRequest request(url);
//     request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
//     request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
//     request.setRawHeader("Accept", "application/vnd.github.v3+json");

//     if (!m_authToken.isEmpty()) {
//         request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
//     }

//     QNetworkReply *reply = m_networkManager->get(request);
//     reply->setProperty("requestType", "searchRepositories");

//     connect(reply, &QNetworkReply::finished, this, &GitHubService ::onSearchResultsReceived);
//     connect(reply,
//             QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
//             this,
//             &GitHubService::onRequestFailed);
// }

void GitHubService::onUserRepositoriesFetched()
{
    qDebug() << "Fetching User Repo 02";
    emit loadingStatusChanged(false);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (!reply || reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Network error: " + (reply ? reply->errorString() : "Unknown"));
        if (reply)
            reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        emit errorOccurred(QString("JSON parsing error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    if (!doc.isArray()) {
        emit errorOccurred("Expected JSON array for repository list");
        reply->deleteLater();
        return;
    }

    QJsonArray repositories = doc.array();

    if (repositories.empty()) {
        qWarning() << "Empty Repositories List";
        return;
    }

    QList<RepositoryItem> repoList;

    foreach (const auto &value, repositories) {
        if (value.isObject()) {
            repoList.append(parseRepositoryJson(value.toObject()));
        }
    }

    emit userRepositoriesFetched(repoList);
    reply->deleteLater();
}

// void GitHubService::onUserRepositoryReceived()
// {
//     QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
//     if (!reply) {
//         setIsLoading(false);
//         setErrorMessage("Invalid response received");
//         return;
//     }

//     setIsLoading(false);

//     if (reply->error() != QNetworkReply::NoError) {
//         reply->deleteLater();
//         return;
//     }

//     QByteArray data = reply->readAll();
//     QJsonParseError parseError;
//     QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

//     if (parseError.error != QJsonParseError::NoError) {
//         setErrorMessage(QString("JSON parsing error: %1").arg(parseError.errorString()));
//         reply->deleteLater();
//         return;
//     }

//     if (!doc.isObject()) {
//         setErrorMessage("Expected JSON object for repository");
//         reply->deleteLater();
//         return;
//     }
//     QVariant repository = parseRepositoryJson(doc.object());
//     emit userrepositoryFetched(repository);
//     reply->deleteLater();
// }

void GitHubService::onRemoteRepositoriesFetched()
{
    qDebug() << "Fetching Remote Repo 02";
    emit loadingStatusChanged(false);
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    if (!reply || reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Network error: " + (reply ? reply->errorString() : "Unknown"));

        if (reply)
            reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        emit errorOccurred(QString("JSON parsing error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    if (!doc.isObject()) {
        emit errorOccurred("Expected JSON object for search results");
        reply->deleteLater();
        return;
    }
    QJsonObject searchResults = doc.object();
    QJsonArray items = searchResults["items"].toArray();

    if (items.empty()) {
        return;
    }

    QList<RepositoryItem> repoList;

    foreach (const QJsonValue &value, items) {
        if (value.isObject()) {
            repoList.append(parseRepositoryJson(value.toObject()));
        }
    }
    emit remoteRepositoriesFetched(repoList);
    reply->deleteLater();
    qDebug() << "Fetching Remote Repo DONE";
}
