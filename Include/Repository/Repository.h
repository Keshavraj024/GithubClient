#pragma once

#include <QDateTime>
#include <QObject>
#include <QString>
#include <qqml.h>

class Repository : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qint64 id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString fullName READ fullName WRITE setFullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString htmlUrl READ htmlUrl WRITE setHtmlUrl NOTIFY htmlUrlChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(int stars READ stars WRITE setStars NOTIFY starsChanged)
    Q_PROPERTY(int forks READ forks WRITE setForks NOTIFY forksChanged)
    Q_PROPERTY(int openIssues READ openIssues WRITE setOpenIssues NOTIFY openIssuesChanged)
    Q_PROPERTY(bool archived READ archived WRITE setArchived NOTIFY archivedChanged)
    Q_PROPERTY(bool isPrivate READ isPrivate WRITE setIsPrivate NOTIFY isPrivateChanged FINAL)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt NOTIFY updatedAtChanged FINAL)

    Q_PROPERTY(QObject *owner READ owner WRITE setOwner NOTIFY ownerChanged)

public:
    explicit Repository(QObject *parent = nullptr);

    QObject *owner() const;
    void setOwner(QObject *newOwner);

    qint64 id() const;
    void setId(qint64 newId);

    QString name() const;
    void setName(const QString &newName);

    QString description() const;
    void setDescription(const QString &newDescription);

    QDateTime updatedAt() const;
    void setUpdatedAt(const QDateTime &newUpdatedAt);

    QString fullName() const;
    void setFullName(const QString &newFullName);

    QString htmlUrl() const;
    void setHtmlUrl(const QString &newHtmlUrl);

    QString language() const;
    void setLanguage(const QString &newLanguage);

    int stars() const;
    void setStars(int newStars);

    int forks() const;
    void setForks(int newForks);

    int openIssues() const;
    void setOpenIssues(int newOpenIssues);

    bool archived() const;
    void setArchived(bool newArchived);

    bool isPrivate() const;
    void setIsPrivate(bool newIsPrivate);

    static Repository *fromJson(const QJsonObject &json, QObject *parent = nullptr);

signals:

    void ownerChanged();

    void idChanged();

    void nameChanged();

    void descriptionChanged();

    void updatedAtChanged();

    void fullNameChanged();

    void htmlUrlChanged();

    void languageChanged();

    void starsChanged();

    void forksChanged();

    void openIssuesChanged();

    void archivedChanged();

    void isPrivateChanged();

public slots:

private:
    QObject *m_owner = nullptr;
    qint64 m_id;
    QString m_name;
    QString m_description;
    QDateTime m_updatedAt;
    QString m_fullName;
    QString m_htmlUrl;
    QString m_language;
    int m_stars;
    int m_forks;
    int m_openIssues;
    bool m_archived;
    bool m_isPrivate;
};
