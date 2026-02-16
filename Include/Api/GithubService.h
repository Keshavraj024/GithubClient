#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>
#include <QQmlEngine>

class GitHubService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString authToken READ authToken WRITE setAuthToken NOTIFY authTokenChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(QVariantList repositories READ repositories NOTIFY repositoriesChanged)

public:
    explicit GitHubService(QObject *parent = nullptr);
    QString authToken() const;
    void setAuthToken(const QString &newAuthToken);

    bool isLoading() const;

    QString errorMessage() const;

    QVariantList repositories() const;

    Q_INVOKABLE void fetchUserRepositories(const QString &username = QString());
    Q_INVOKABLE void fetchRepository(const QString &user, const QString &repo);
    Q_INVOKABLE void clearRepositories();
    Q_INVOKABLE void searchRepositories(const QString &query,
                                        const QString &sort = "stars",
                                        const QString &order = "desc");

    Q_INVOKABLE void fetchAuthenticatedUserRepositories();
    Q_INVOKABLE void fetchTrendingRepositories(const int days = 7);

signals:
    void authTokenChanged();

    void isLoadingChanged();

    void errorMessageChanged();

    void repositoriesChanged();

    void userrepositoryFetched(const QVariant &repository);

private slots:
    void onUserRepositoriesReceived();
    void onUserRepositoryReceived();
    void onSearchResultsReceived();
    void onRequestFailed(QNetworkReply::NetworkError error);

private:
    QNetworkAccessManager *m_networkManager;

    QString m_authToken;
    bool m_isLoading{false};
    QString m_errorMessage;
    QVariantList m_repositories;

private:
    void setIsLoading(bool loading);
    void setErrorMessage(const QString &message);
    QVariant parseRepositoryJson(const QJsonObject &json);
};
