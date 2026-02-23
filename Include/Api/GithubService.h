#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>
#include "RepositoryItem.h"

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
    void onUserRepositoriesFetched();
    void onUserRepositoryFetched();
    void onRemoteRepositoriesFetched();

    void onGithubStatusFetched();

private:
    QNetworkAccessManager *m_networkManager;

private:
    RepositoryItem parseRepositoryJson(const QJsonObject &json);
};
