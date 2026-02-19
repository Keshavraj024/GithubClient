#include "RepositoryController.h"
#include <QDebug>
#include <QSqlDatabase>

RepositoryController::RepositoryController(QObject *parent)
    : QObject(parent)
{
    // 1. Initialize Storage with the default connection
    m_storage = std::make_unique<RepositoryStorage>(QSqlDatabase::database("github_connection"));
    m_gitService = new GitHubService(this);

    connect(m_gitService,
            &GitHubService::userRepositoriesFetched,
            this,
            &RepositoryController::onUserRepositoriesFetched);

    // 2. Load initially saved items into the model at startup
    refreshSavedItems();
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
    m_gitService->fetchUserRepositories(username, m_authToken);
}

void RepositoryController::onUserRepositoriesFetched(const QList<RepositoryItem> repoItems)
{
    qDebug() << "Fetching User Repo 03" << repoItems[0].id;
    QList<RepositoryItem> finalResults = repoItems;
    reconcileWithDatabase(finalResults);
    m_model.updateFromApi(finalResults);
    setIsLoading(false);
}

// void RepositoryController::search(const QString &query)
// {
//     if (query.isEmpty()) return;

//     // 1. Clear previous search results (but keep saved items)
//     m_model.clearSearch();

//     // 2. SIMULATED API CALL (Replace with your actual Network Manager logic)
//     // For now, imagine 'results' is the list returned from GitHub
//     QList<RepositoryItem> results;

//     // 3. Reconcile: Check if any of these API results are already in our DB
//     reconcileWithDatabase(results);

//     // 4. Update the Model
//     m_model.updateFromApi(results);
// }

void RepositoryController::reconcileWithDatabase(QList<RepositoryItem> &apiResults)
{
    for (auto &item : apiResults) {
        if (m_storage->exists(item.id)) {
            item.isLocallySaved = true;
            // You could also load the specific saved date if needed
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
    if (!message.isEmpty()) {
        m_errorMessage = message;
        emit errorMessageChanged();
    }
}

void RepositoryController::toggleSave(int index)
{
    // Get a reference to the item in the model's list
    RepositoryItem &item = m_model.getItem(index);

    if (!item.isLocallySaved) {
        // ACTION: SAVE
        if (m_storage->saveRepo(item)) {
            item.isLocallySaved = true;
            m_model.notifyRowChanged(index);
            qDebug() << "Saved repo:" << item.fullName;
        }
    } else {
        // ACTION: REMOVE
        if (m_storage->removeRepo(item.id)) {
            item.isLocallySaved = false;
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
