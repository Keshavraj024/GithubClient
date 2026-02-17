#pragma once

#include <QSqlDatabase>

class DatabaseManager
{
public:
    [[nodiscard]] static bool initialize();
    static QSqlDatabase database();

private:
    static bool createTables();
};
