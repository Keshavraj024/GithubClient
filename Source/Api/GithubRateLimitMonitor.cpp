#include "GithubRateLimitMonitor.h"

GitHubRateLimitMonitor::GitHubRateLimitMonitor(QObject *parent)
    : QObject(parent)
{}

void GitHubRateLimitMonitor::processReply(QNetworkReply *reply)
{
    if (!reply || !reply->hasRawHeader("x-ratelimit-remaining"))
        return;

    bool isSearch = reply->url().path().contains("/search/");

    int remaining = reply->rawHeader("x-ratelimit-remaining").toInt();
    int limit = reply->rawHeader("x-ratelimit-limit").toInt();
    qint64 reset = reply->rawHeader("x-ratelimit-reset").toLongLong();

    // If limit > 60 for Core, or limit > 10 for Search, they are authenticated
    bool currentlyAuth = isSearch ? (limit > 10) : (limit > 60);
    if (currentlyAuth != m_isAuthenticated) {
        m_isAuthenticated = currentlyAuth;
        emit authStatusChanged();
    }

    // 4. Update the correct bucket
    if (isSearch) {
        // qDebug() << "Searching " << remaining << " " << limit;
        m_searchRemaining = remaining;
        m_searchLimit = limit;
        m_searchReset = reset;
        if (m_searchRemaining == 0)
            emit searchRateExhausted(m_searchReset);
    } else {
        // qDebug() << "Users " << remaining << " " << limit;
        m_coreRemaining = remaining;
        m_coreLimit = limit;
        m_coreReset = reset;
        if (m_coreRemaining == 0)
            emit coreRateExhausted(m_coreReset);
    }

    emit rateUpdated();
}

int GitHubRateLimitMonitor::coreRemaining() const
{
    return m_coreRemaining;
}
int GitHubRateLimitMonitor::searchRemaining() const
{
    return m_searchRemaining;
}
bool GitHubRateLimitMonitor::isAuthenticated() const
{
    return m_isAuthenticated;
}
