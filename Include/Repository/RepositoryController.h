#pragma once
#include <QObject>
#include "GithubService.h"
#include "RepositoryModel.h"
#include "RepositoryStorage.h"
#include <memory>

class RepositoryController : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString authToken WRITE setAuthToken MEMBER m_authToken NOTIFY authTokenChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(RepositoryModel *model READ model CONSTANT)

public:
    explicit RepositoryController(QObject *parent = nullptr);

    RepositoryModel *model();

    // Logic accessible from QML
    Q_INVOKABLE void fetchUserRepositories(const QString &username);

    // Q_INVOKABLE void fetchRepository(const QString &user, const QString &repo);
    // Q_INVOKABLE void clearRepositories();
    // Q_INVOKABLE void searchRepositories(const QString &query,
    //                                     const QString &sort = "stars",
    //                                     const QString &order = "desc");

    // Q_INVOKABLE void fetchAuthenticatedUserRepositories();
    // Q_INVOKABLE void fetchTrendingRepositories(const int days = 7);

    Q_INVOKABLE void toggleSave(int index);
    Q_INVOKABLE void refreshSavedItems();

    void setAuthToken(const QString &newAuthToken);

    bool isLoading() const;

    QString errorMessage() const;

signals:
    void authTokenChanged();

    void isLoadingChanged();

    void errorMessageChanged();

private slots:
    void onUserRepositoriesFetched(const QList<RepositoryItem> repoItems);
    void onRequestFailed(QNetworkReply::NetworkError error);

private:
    RepositoryModel m_model;
    std::unique_ptr<RepositoryStorage> m_storage;
    GitHubService *m_gitService;

    // Internal helper to sync DB status with API results
    void reconcileWithDatabase(QList<RepositoryItem> &apiResults);
    void setIsLoading(bool loading);
    void setErrorMessage(const QString &message);

    QString m_authToken;
    bool m_isLoading{false};
    QString m_errorMessage;
};
