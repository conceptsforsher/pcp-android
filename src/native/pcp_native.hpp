#pragma once

#include <QObject>
#include <QString>
#include <QImage>
#include <QMap>

namespace pcp_chat {

class PcpNative : public QObject
{
    Q_OBJECT

public:
    explicit PcpNative(QObject *parent = nullptr);
    ~PcpNative() override;

    // Platform detection
    static bool isLinux();
    static bool isMacOS();
    static bool isWindows();
    static bool isWayland();
    static bool isX11();

    // System info
    static QString osName();
    static QString osVersion();
    static QString desktopEnvironment();
    static QString hostname();
    static qint64 totalMemory();
    static qint64 availableMemory();
    static int cpuCores();

    // Process management
    static void openUrl(const QString &url);
    static void openFileManager(const QString &path);
    static void setAutoStart(bool enabled);
    static bool isAutoStartEnabled();

    // Screenshot
    static QImage takeScreenshot();

    // Notifications
    static bool nativeNotificationsSupported();
    static void showNativeNotification(const QString &title, const QString &body,
                                        const QString &iconPath = "");

    // File dialogs
    static QString saveFileDialog(const QString &title, const QString &defaultName,
                                   const QString &filter);
    static QString openFileDialog(const QString &title, const QString &filter);
    static QString openDirectoryDialog(const QString &title);

signals:
    void screenLocked();
    void screenUnlocked();
    void systemSleep();
    void systemWake();

private:
    void setupPlatformHooks();
};

} // namespace pcp_chat
