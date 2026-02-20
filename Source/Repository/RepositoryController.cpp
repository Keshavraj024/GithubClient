#include "RepositoryController.h"
#include <QDebug>
#include <QSqlDatabase>

RepositoryController::RepositoryController(QObject *parent)
    : QObject(parent)
{
    m_storage = std::make_unique<RepositoryStorage>(QSqlDatabase::database("github_connection"));
    m_gitService = new GitHubService(this);

    connect(m_gitService,
            &GitHubService::loadingStatusChanged,
            this,
            &RepositoryController::setIsLoading);

    connect(m_gitService,
            &GitHubService::errorOccurred,
            this,
            &RepositoryController::setErrorMessage);

    fetchRemoteRepositories("stars:>10000", "stars", "desc");
    // refreshSavedItems();
}

RepositoryModel *RepositoryController::model()
{
    return &m_model;
}

void RepositoryController::refreshSavedItems()
{
    QList<RepositoryItem> saved = m_storage->loadAll();
    m_model.loadFromDatabase(saved);
}

void RepositoryController::fetchUserRepositories(const QString &username)
{
    if (m_isLoading || username.isEmpty()) {
        return;
    }
    qDebug() << "Fetching User Repo";
    setIsLoading(true);
    setErrorMessage(QString());
    m_model.clearSearch();
    m_gitService->fetchUserRepositories(username);

    connect(m_gitService,
            &GitHubService::userRepositoriesFetched,
            this,
            &RepositoryController::onUserRepositoriesFetched);
}

void RepositoryController::fetchAuthenticatedUserRepositories()
{
    if (m_isLoading || m_authToken.isEmpty()) {
        setErrorMessage("Invalid Token");
        return;
    }
    setIsLoading(true);
    setErrorMessage(QString());
    m_model.clearSearch();
    m_gitService->fetchAuthenticatedUserRepositories(m_authToken);

    connect(m_gitService,
            &GitHubService::userRepositoriesFetched,
            this,
            &RepositoryController::onUserRepositoriesFetched);
}

void RepositoryController::onUserRepositoriesFetched(const QList<RepositoryItem> repoItems)
{
    qDebug() << "Fetching User Repo 03" << repoItems[0].id;
    QList<RepositoryItem> finalResults = repoItems;
    reconcileWithDatabase(finalResults);
    m_model.updateFromApi(finalResults);
    emit modelCountChanged();
}

void RepositoryController::fetchRemoteRepositories(const QString &query,
                                                   const QString &sort,
                                                   const QString &order)
{
    if (m_isLoading || query.isEmpty()) {
        return;
    }

    qDebug() << "Fetching Remote Repo";
    setIsLoading(true);
    setErrorMessage(QString());
    m_model.clearSearch();
    m_gitService->fetchRemoteRepositories(query, sort, order);

    connect(m_gitService,
            &GitHubService::remoteRepositoriesFetched,
            this,
            &RepositoryController::onRemoteRepositoriesFetched);
}

void RepositoryController::onRemoteRepositoriesFetched(const QList<RepositoryItem> repoItems)
{
    qDebug() << "Fetching User Repo 03" << repoItems[0].id;
    QList<RepositoryItem> finalResults = repoItems;
    reconcileWithDatabase(finalResults);
    m_model.updateFromApi(finalResults);
    emit modelCountChanged();
}

void RepositoryController::reconcileWithDatabase(QList<RepositoryItem> &apiResults)
{
    QMap<qlonglong, QString> savedDates = m_storage->getAllSavedDates();

    for (auto &item : apiResults) {
        if (savedDates.contains(item.id)) {
            item.isLocallySaved = true;
            item.savedAt = savedDates.value(item.id);
        } else {
            item.isLocallySaved = false;
            item.savedAt = "";
        }
    }
}

void RepositoryController::setIsLoading(bool loading)
{
    if (m_isLoading == loading)
        return;
    m_isLoading = loading;
    emit isLoadingChanged();
}

void RepositoryController::setErrorMessage(const QString &message)
{
    if (m_errorMessage == message)
        return;

    m_errorMessage = message;

    emit modelCountChanged();
    emit errorMessageChanged();
}

void RepositoryController::toggleSave(int index)
{
    RepositoryItem &item = m_model.getItem(index);

    if (!item.isLocallySaved) {
        if (m_storage->saveRepo(item)) {
            item.isLocallySaved = true;
            item.savedAt = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
            m_model.notifyRowChanged(index);
            qDebug() << "Saved repo:" << item.fullName;
        }
    } else {
        if (m_storage->removeRepo(item.id)) {
            item.isLocallySaved = false;
            item.savedAt = "";
            m_model.notifyRowChanged(index);
            qDebug() << "Removed repo:" << item.fullName;
        }
    }
}

void RepositoryController::setAuthToken(const QString &newAuthToken)
{
    if (m_authToken == newAuthToken)
        return;
    m_authToken = newAuthToken;
    emit authTokenChanged();
}

bool RepositoryController::isLoading() const
{
    return m_isLoading;
}

QString RepositoryController::errorMessage() const
{
    return m_errorMessage;
}

void RepositoryController::onRequestFailed(QNetworkReply::NetworkError error)
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

size_t RepositoryController::modelCount() const
{
    return m_model.rowCount();
}
