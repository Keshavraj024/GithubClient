#pragma once

#include <QDateTime>
#include <QMetaType>
#include <QString>
#include "UserData.h"

struct RepositoryItem
{
    // API & DB Shared Fields
    qint64 id = 0;
    QString name;
    QString fullName;
    QString description;
    QString htmlUrl;
    QString language;
    int stars = 0;
    int forks = 0;
    int openIssues = 0;
    bool archived = false;
    bool isPrivate = false;
    QDateTime updatedAt;

    // Nested User Info
    UserData owner;

    // Database specific fields
    int collectionId = -1;
    QString savedAt;

    // Logic flag for UI
    bool isLocallySaved = false;
};
Q_DECLARE_METATYPE(RepositoryItem)
