#include "GithubService.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>

#include <QNetworkReplyWrapper>

GitHubService::GitHubService(QObject *parent)
    : QObject{parent}
    , m_networkManager(new QNetworkAccessManager(this))
    , m_rateLimitMonitor(new GitHubRateLimitMonitor(this))

{}

void GitHubService::fetchUserRepositories(const QString &username)
{
    const auto prepareRequest = [this, username](QtTaskTree::QNetworkReplyWrapper &task) {
        qDebug() << "Preparing request for user: Keshav";

        QString endpoint = QString("users/%1/repos").arg(username);
        QUrl url(QString("https://api.github.com/%1?sort=updated&per_page=50").arg(endpoint));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
        request.setRawHeader("Accept", "application/vnd.github.v3+json");

        if (!this->m_networkManager)
            qWarning() << "Network manager is not set";

        task.setNetworkAccessManager(this->m_networkManager);
        task.setRequest(request);
    };

    executeRepositoryRequest(prepareRequest);
}

void GitHubService::fetchRepository(const QString &user, const QString &repo)
{
    const auto prepareRequest = [this, user, repo](QtTaskTree::QNetworkReplyWrapper &task) {
        QUrl url(QString("https://api.github.com/repos/%1/%2").arg(user, repo));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
        request.setRawHeader("Accept", "application/vnd.github.v3+json");

        if (!this->m_networkManager)
            qWarning() << "Network manager is not set";

        task.setNetworkAccessManager(this->m_networkManager);
        task.setRequest(request);
    };

    executeRepositoryRequest(prepareRequest);
}

static void parseRepos(QPromise<QList<RepositoryItem>> &promise, const QByteArray &data)
{
    qDebug() << "Parsing Repos";

    QList<RepositoryItem> repos;

    QJsonDocument doc = QJsonDocument::fromJson(data);

    QJsonArray repositoriesArray;

    if (doc.isObject()) {
        const QJsonObject obj = doc.object();

        if (obj.contains("items") && obj.value("items").isArray()) {
            // Search response
            repositoriesArray = obj.value("items").toArray();
        } else {
            // Single repository object → wrap it into array
            repositoriesArray = QJsonArray{obj};
        }
    } else if (doc.isArray()) {
        repositoriesArray = doc.array();
    } else {
        qWarning() << "Unexpected JSON structure";
    }

    repos.reserve(repositoriesArray.size());

    foreach (const auto &val, repositoriesArray) {
        QJsonObject json = val.toObject();

        RepositoryItem repo;

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

        if (json.contains("owner") && json["owner"].isObject()) {
            QJsonObject ownerJson = json["owner"].toObject();

            repo.owner.id = ownerJson["id"].toVariant().toLongLong();
            repo.owner.login = ownerJson["login"].toString();
            repo.owner.avatarUrl = ownerJson["avatar_url"].toString();
            repo.owner.htmlUrl = ownerJson["html_url"].toString();
            repo.owner.type = ownerJson["type"].toString();
        } else {
            qWarning() << "Owner data missing for repo:" << repo.fullName;
        }

        repos.append(repo);
    }
    qDebug() << "Repos length " << repos.size();
    promise.addResult(repos);
}

void GitHubService::executeRepositoryRequest(
    std::function<void(QtTaskTree::QNetworkReplyWrapper &)> prepareRequest)
{
    emit loadingStatusChanged(true);

    const auto processResponse = [this](const QtTaskTree::QNetworkReplyWrapper &task,
                                        QtTaskTree::DoneWith result) {
        if (result == QtTaskTree::DoneWith::Success) {
            m_storage->data = task.reply()->readAll();
        } else {
            m_lastError = task.reply()->errorString();
        }
    };

    const auto onParseSetup = [this](QtTaskTree::QThreadFunction<QList<RepositoryItem>> &task) {
        task.setThreadFunctionData(&parseRepos, m_storage->data);
    };

    const auto onParseDone = [this](const QtTaskTree::QThreadFunction<QList<RepositoryItem>> &task,
                                    QtTaskTree::DoneWith result) {
        if (result != QtTaskTree::DoneWith::Success)
            return;

        emit remoteRepositoriesFetched(task.result());
    };

    const auto finalizeSession = [this](QtTaskTree::DoneWith result) {
        emit loadingStatusChanged(false);

        if (result == QtTaskTree::DoneWith::Error)
            emit errorOccurred(m_lastError);
    };

    const QtTaskTree::Group recipe{
        m_storage,

        QtTaskTree::For(QtTaskTree::RepeatIterator(3))
            >> QtTaskTree::Do{QtTaskTree::stopOnSuccess,
                              QtTaskTree::QNetworkReplyWrapperTask(prepareRequest, processResponse),
                              timeoutTask(std::chrono::seconds(5), QtTaskTree::DoneResult::Error)},

        QtTaskTree::QThreadFunctionTask<QList<RepositoryItem>>(onParseSetup, onParseDone),
        QtTaskTree::onGroupDone(finalizeSession)};

    m_taskTree.setRecipe(recipe);
    m_taskTree.start();
}

void GitHubService::fetchRemoteRepositories(const QString &query,
                                            const QString &sort,
                                            const QString &order)
{
    const auto prepareRequest = [this, query, sort, order](QtTaskTree::QNetworkReplyWrapper &task) {
        qDebug() << "Preparing request for user: Keshav";

        QUrl url(
            QString("https://api.github.com/search/repositories?q=%1&sort=%2&order=%3&per_page=50")
                .arg(query, sort, order));

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
        request.setRawHeader("Accept", "application/vnd.github.v3+json");

        if (!this->m_networkManager)
            qWarning() << "Network manager is not set";

        task.setNetworkAccessManager(this->m_networkManager);
        task.setRequest(request);
    };

    executeRepositoryRequest(prepareRequest);
}

void GitHubService::fetchAuthenticatedUserRepositories(const QString &authToken)
{
    const auto prepareRequest = [this, authToken](QtTaskTree::QNetworkReplyWrapper &task) {
        qDebug() << "Preparing request for user: Keshav";

        QUrl url(
            QString("https://api.github.com/user/repos?sort=updated&per_page=50&visibility=all"));
        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
        request.setRawHeader("Accept", "application/vnd.github.v3+json");
        request.setRawHeader("Authorization", QString("Bearer %1").arg(authToken).toUtf8());

        if (!this->m_networkManager)
            qWarning() << "Network manager is not set";

        task.setNetworkAccessManager(this->m_networkManager);
        task.setRequest(request);
    };

    executeRepositoryRequest(prepareRequest);
}

void GitHubService::fetchGithubStatus()
{
    QUrl url(QString("https://www.githubstatus.com/api/v2/status.json"));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("User-Agent", "GitHubClient-Qt-Modern");
    request.setRawHeader("Accept", "application/vnd.github.v3+json");

    QNetworkReply *reply = m_networkManager->get(request);
    reply->setProperty("requestType", "githubAPIStatus");

    connect(reply, &QNetworkReply::finished, this, &GitHubService ::onGithubStatusFetched);
    connect(reply,
            QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply ::errorOccurred),
            this,
            &GitHubService::requestFailed);
}

void GitHubService::onGithubStatusFetched()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
    if (!reply)
        return;

    if (!reply || reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Network error: " + (reply ? reply->errorString() : "Unknown"));
        if (reply)
            reply->deleteLater();
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject root = doc.object();
    QJsonObject statusObj = root["status"].toObject();

    QString indicator = statusObj["indicator"].toString();
    QString description = statusObj["description"].toString();

    qDebug() << indicator;

    // bool isOnline = (indicator == "none");

    emit githubStatusFetched(indicator, description);
    reply->deleteLater();
}
