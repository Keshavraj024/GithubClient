#pragma once

#include <QString>

struct UserData
{
    qint64 id = 0;
    QString login;
    QString avatarUrl;
    QString htmlUrl;
    QString type;
};
