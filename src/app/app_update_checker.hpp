#pragma once

#include <QObject>
#include <QString>
#include <QScopedPointer>
#include <QVersionNumber>

class QNetworkAccessManager;
class QNetworkReply;
class QTimer;

namespace pcp {

class AppUpdateChecker : public QObject
{
    Q_OBJECT

public:
    static AppUpdateChecker &instance();

    void checkForUpdates();
    void checkForUpdatesPeriodically();

    void setCurrentVersion(const QString &version);
    QString currentVersion() const;

    bool isUpdateAvailable() const;
    QString latestVersion() const;
    QString downloadUrl() const;
    QString releaseNotes() const;

    void openDownloadPage();

signals:
    void updateAvailable(const QString &latestVersion,
                         const QString &downloadUrl,
                         const QString &releaseNotes);
    void noUpdateAvailable();
    void checkFailed(const QString &error);

private:
    explicit AppUpdateChecker(QObject *parent = nullptr);
    ~AppUpdateChecker() override;
    Q_DISABLE_COPY(AppUpdateChecker)

    void onReplyFinished(QNetworkReply *reply);

    QNetworkAccessManager *m_nam;
    QTimer *m_periodicTimer;
    QString m_currentVersion;
    QString m_latestVersion;
    QString m_downloadUrl;
    QString m_releaseNotes;
    bool m_updateAvailable = false;
};

} // namespace pcp
