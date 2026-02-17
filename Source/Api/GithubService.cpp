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
    fetchTrendingRepositories();
}

QString GitHubService::authToken() const
{
    return m_authToken;
}

void GitHubService::setAuthToken(const QString &newAuthToken)
{
    if (m_authToken == newAuthToken)
        return;
    m_authToken = newAuthToken;
    emit authTokenChanged();
}

bool GitHubService::isLoading() const
{
    return m_isLoading;
}

void GitHubService::setIsLoading(bool loading)
{
    if (m_isLoading == loading)
        return;

    m_isLoading = loading;
    emit isLoadingChanged();
}

QString GitHubService::errorMessage() const
{
    return m_errorMessage;
}

void GitHubService::setErrorMessage(const QString &message)
{
    if (m_errorMessage == message)
        return;

    m_errorMessage = message;
    emit errorMessageChanged();
}

QVariant GitHubService::parseRepositoryJson(const QJsonObject &json)
{
    QVariantMap repo;
    repo["id"] = json["id"].toVariant().toLongLong();
    repo["name"] = json["name"].toString();
    repo["fullName"] = json["full_name"].toString();
    repo["stars"] = json["stargazers_count"].toInt();
    repo["forks"] = json["forks_count"].toInt();
    repo["isPrivate"] = json["private"].toBool();
    repo["htmlUrl"] = json["html_url"].toString();
    repo["openIssues"] = json["open_issues_count"].toInt();
    repo["archived"] = json["archived"].toBool();

    if (!json["description"].isNull())
        repo["description"] = json["description"].toString();

    repo["language"] = json["language"].toString();

    QDateTime updated = QDateTime::fromString(json["updated_at"].toString(), Qt ::ISODate);

    if (updated.isValid())
        repo["updatedAt"] = updated;

    // User information
    if (json.contains("owner") && json["owner"].isObject()) {
        QJsonObject owner = json["owner"].toObject();
        QVariantMap ownerMap;
        ownerMap["id"] = owner["id"].toInt();
        ownerMap["login"] = owner["login"].toString();
        ownerMap["avatarUrl"] = owner["avatar_url"].toString();
        ownerMap["htmlUrl"] = owner["html_url"].toString();
        ownerMap["type"] = owner["type"].toString();
        repo["owner"] = ownerMap;
    } else {
        qWarning() << "Owner doesn't exists \n";
    }

    return repo;
}

QVariantList GitHubService::repositories() const
{
    return m_repositories;
}

void GitHubService::fetchUserRepositories(const QString &username)
{
    if (m_isLoading || username.isEmpty()) {
        return;
    }

    setIsLoading(true);
    setErrorMessage(QString());
    clearRepositories();

    QString endpoint = QString("users/%1/repos").arg(username);
    QUrl url(QString("https://api.github.com/%1?sort=updated&per_page=50").arg(endpoint));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "userRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onUserRepositoriesReceived);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::onRequestFailed);
}

void GitHubService::fetchRepository(const QString &user, const QString &repo)
{
    if (m_isLoading || user.isEmpty() || repo.isEmpty()) {
        return;
    }

    setIsLoading(true);
    setErrorMessage(QString());
    clearRepositories();

    QUrl url(QString("https://api.github.com/repos/%1/%2").arg(user, repo));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "userSingleRepository");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onUserRepositoryReceived);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::onRequestFailed);
}

void GitHubService::clearRepositories()
{
    if (!m_repositories.isEmpty()) {
        m_repositories.clear();
        emit repositoriesChanged();
    }
}

void GitHubService::searchRepositories(const QString &query,
                                       const QString &sort,
                                       const QString &order)
{
    if (m_isLoading || query.isEmpty()) {
        return;
    }

    setIsLoading(true);
    setErrorMessage(QString());
    clearRepositories();

    QUrl url(QString("https://api.github.com/search/repositories?q=%1&sort=%2&order=%3&per_page=50")
                 .arg(query, sort, order));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    // TODO : if this field is not empty then it throws error
    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "searchRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onSearchResultsReceived);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::onRequestFailed);
}

void GitHubService::fetchAuthenticatedUserRepositories()
{
    if (m_isLoading) {
        return;
    }

    if (m_authToken.isEmpty()) {
        qWarning() << "Token is not valid";
    }

    setIsLoading(true);
    setErrorMessage(QString());
    clearRepositories();

    QUrl url(QString("https://api.github.com/user/repos?sort=updated&per_page=50&visibility=all"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "userRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onUserRepositoriesReceived);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::onRequestFailed);
}

void GitHubService::fetchTrendingRepositories(const int days)
{
    if (m_isLoading) {
        return;
    }

    setIsLoading(true);
    setErrorMessage(QString());
    clearRepositories();

    QDate date = QDate::currentDate().addDays(-days);
    QString query = QString("created:>%1").arg(date.toString("yyyy-MM-dd"));

    QUrl url(
        QString("https://api.github.com/search/repositories?q=%1&sort=stars&order=desc&per_page=50")
            .arg(query));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    if (!m_authToken.isEmpty()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_authToken).toUtf8());
    }

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "searchRepositories");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onSearchResultsReceived);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::onRequestFailed);
}

void GitHubService::onUserRepositoriesReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        setIsLoading(false);
        setErrorMessage("Invalid response received");
        return;
    }

    setIsLoading(false);

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        setErrorMessage(QString("JSON parsing error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    if (!doc.isArray()) {
        setErrorMessage("Expected JSON array for repository list");
        reply->deleteLater();
        return;
    }

    QJsonArray repositories = doc.array();
    QVariantList repoList;
    foreach (const auto &value, repositories) {
        if (value.isObject()) {
            repoList.append(parseRepositoryJson(value.toObject()));
        }
    }
    m_repositories = repoList;
    emit repositoriesChanged();
    reply->deleteLater();
}

void GitHubService::onUserRepositoryReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        setIsLoading(false);
        setErrorMessage("Invalid response received");
        return;
    }

    setIsLoading(false);

    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        setErrorMessage(QString("JSON parsing error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }

    if (!doc.isObject()) {
        setErrorMessage("Expected JSON object for repository");
        reply->deleteLater();
        return;
    }
    QVariant repository = parseRepositoryJson(doc.object());
    emit userrepositoryFetched(repository);
    reply->deleteLater();
}

void GitHubService::onSearchResultsReceived()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply) {
        setIsLoading(false);
        setErrorMessage("Invalid response received");
        return;
    }
    setIsLoading(false);
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }
    QByteArray data = reply->readAll();
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        setErrorMessage(QString("JSON parsing error: %1").arg(parseError.errorString()));
        reply->deleteLater();
        return;
    }
    if (!doc.isObject()) {
        setErrorMessage("Expected JSON object for search results");
        reply->deleteLater();
        return;
    }
    QJsonObject searchResults = doc.object();
    QJsonArray items = searchResults["items"].toArray();
    QVariantList repoList;

    foreach (const QJsonValue &value, items) {
        if (value.isObject()) {
            repoList.append(parseRepositoryJson(value.toObject()));
        }
    }
    m_repositories = repoList;
    emit repositoriesChanged();
    reply->deleteLater();
}

void GitHubService::onRequestFailed(QNetworkReply::NetworkError error)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());

    setIsLoading(false);
    QString errorMsg;
    switch (error) {
    case QNetworkReply::AuthenticationRequiredError:
        errorMsg = "Authentication required. Please check your token.";
        break;
    case QNetworkReply::ContentNotFoundError:
        errorMsg = "Repository or user not found.";
        break;
    case QNetworkReply::TimeoutError:
        errorMsg = "Request timed out. Please try again.";
        break;
    case QNetworkReply::HostNotFoundError:
        errorMsg = "Unable to connect to GitHub API.";
        break;
    default:
        errorMsg = QString("Network error occurred: " + reply->errorString());
        break;
    }
    setErrorMessage(errorMsg);
    qWarning() << "GitHub API request failed:" << errorMsg;

    if (reply) {
        reply->deleteLater();
    }
}
