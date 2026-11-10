#pragma once

#include <QString>
#include <QObject>
#include <QVersionNumber>

namespace pcp_chat {

class AppUpdateChecker : public QObject
{
    Q_OBJECT

public:
    struct UpdateInfo {
        bool available = false;
        QString version;
        QString url;
        QString changelog;
        bool mandatory = false;
        qint64 sizeBytes = 0;
    };

    static void checkForUpdates();
    static UpdateInfo latestUpdateInfo();
    static bool isUpdateAvailable();
    static void downloadUpdate();

signals:
    void updateAvailable(const UpdateInfo &info);
    void downloadProgress(qint64 received, qint64 total);
    void downloadComplete(const QString &path);

private:
    static AppUpdateChecker &instance();

    explicit AppUpdateChecker(QObject *parent = nullptr);
    void performCheck();
    void parseResponse(const QByteArray &data);

    UpdateInfo m_latestInfo;
    bool m_checked = false;
};

} // namespace pcp_chat
