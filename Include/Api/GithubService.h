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

    void fetchUserRepositories(const QString &username, const QString &authToken);

    // void fetchRepository(const QString &user, const QString &repo);
    // void clearRepositories();
    // void searchRepositories(const QString &query,
    //                         const QString &sort = "stars",
    //                         const QString &order = "desc");

    // void fetchAuthenticatedUserRepositories();
    // void fetchTrendingRepositories(const int days = 7);

signals:
    // void userrepositoryFetched(const QVariant &repository);

    void userRepositoriesFetched(const QList<RepositoryItem> repoItems);

    void requestFailed(QNetworkReply::NetworkError error);

private slots:
    void onUserRepositoriesReceived();
    // void onUserRepositoryReceived();
    // void onSearchResultsReceived();
    // void onRequestFailed(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;

private:
    RepositoryItem parseRepositoryJson(const QJsonObject &json);
};
