#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

class GitHubRateLimitMonitor : public QObject
{
    Q_OBJECT
    // We expose properties for both "buckets" so the UI can update the correct widget
    Q_PROPERTY(int coreRemaining READ coreRemaining NOTIFY rateUpdated)
    Q_PROPERTY(int searchRemaining READ searchRemaining NOTIFY rateUpdated)
    Q_PROPERTY(bool isAuthenticated READ isAuthenticated NOTIFY authStatusChanged)

public:
    explicit GitHubRateLimitMonitor(QObject *parent = nullptr);

    // This now determines which bucket to update based on the reply URL
    void processReply(QNetworkReply *reply);

    // Getters
    int coreRemaining() const;
    int searchRemaining() const;
    bool isAuthenticated() const;

signals:
    void rateUpdated();
    void authStatusChanged();
    void coreRateExhausted(qint64 resetEpoch);
    void searchRateExhausted(qint64 resetEpoch);

private:
    // Core API (Repos, Issues, Users) - 60/hr or 5000/hr
    int m_coreRemaining = 60;
    int m_coreLimit = 60;
    qint64 m_coreReset = 0;

    // Search API - 10/min or 30/min
    int m_searchRemaining = 10;
    int m_searchLimit = 10;
    qint64 m_searchReset = 0;

    bool m_isAuthenticated = false;
};
