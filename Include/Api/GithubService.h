#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include "GithubRateLimitMonitor.h"
#include "RepositoryItem.h"

#include <QtTaskTree>

class GitHubService : public QObject
{
    Q_OBJECT

public:
    explicit GitHubService(QObject *parent = nullptr);

    void fetchUserRepositories(const QString &username);

    void fetchRepository(const QString &user, const QString &repo);

    void fetchRemoteRepositories(const QString &query,
                                 const QString &sort = "stars",
                                 const QString &order = "desc");

    void fetchAuthenticatedUserRepositories(const QString &authToken);

    void fetchGithubStatus();

signals:
    void userRepositoryFetched(const RepositoryItem &repository);

    void userRepositoriesFetched(const QList<RepositoryItem> repoItems);
    void remoteRepositoriesFetched(const QList<RepositoryItem> repoItems);

    void githubStatusFetched(const QString &status, const QString &desc);

    void errorOccurred(const QString &errorMsg);
    void loadingStatusChanged(bool isLoading);

    void requestFailed(QNetworkReply::NetworkError error);

private slots:
    void onGithubStatusFetched();

private:
    struct RepositoryRequestContext
    {
        QByteArray data;
    };

    QtTaskTree::Storage<RepositoryRequestContext> m_storage;

    QNetworkAccessManager *m_networkManager;
    GitHubRateLimitMonitor *m_rateLimitMonitor;

    QtTaskTree::QTaskTree m_taskTree;
    QString m_lastError;

private:
    void executeRepositoryRequest(
        std::function<void(QtTaskTree::QNetworkReplyWrapper &)> prepareRequest);
};
