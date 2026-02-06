#pragma once

#include <QJsonObject>
#include <QObject>
#include <QString>
#include <qqmlintegration.h>

class User : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged FINAL)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
    Q_PROPERTY(QString avatarUrl READ avatarUrl WRITE setAvatarUrl NOTIFY avatarUrlChanged)
    Q_PROPERTY(QString htmlUrl READ htmlUrl WRITE setHtmlUrl NOTIFY htmlUrlChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)

public:
    explicit User(QObject *parent = nullptr);

    qint64 id() const;
    void setId(qint64 newId);
    QString login() const;
    void setLogin(const QString &newLogin);

    QString avatarUrl() const;
    void setAvatarUrl(const QString &newAvatarUrl);

    QString htmlUrl() const;
    void setHtmlUrl(const QString &newHtmlUrl);

    QString type() const;
    void setType(const QString &newType);

    static User *fromJson(const QJsonObject &json, QObject *parent = nullptr);

signals:
    void idChanged();

    void loginChanged();

    void avatarUrlChanged();

    void htmlUrlChanged();

    void typeChanged();

private:
    int m_id;
    QString m_login;
    QString m_avatarUrl;
    QString m_htmlUrl;
    QString m_type;
};
