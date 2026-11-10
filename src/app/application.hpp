#pragma once

#include <QApplication>
#include <QString>
#include <memory>

class QNetworkAccessManager;
class QNetworkProxy;

namespace pcp_chat {

class MainWindow;
class NotificationManager;
class ThemeManager;
class ProtocolManager;

// Application shell — owns all top-level subsystems and manages lifecycle
class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);
    ~Application() override;

    // Configuration
    void setConfigDirectory(const QString &path);
    QString configDirectory() const;

    void setDebugLogging(bool enabled);
    void setLogFile(const QString &path);
    void setCrashReportingEnabled(bool enabled);
    void setProxyUrl(const QString &url);

    // Lifecycle
    bool initialize();
    int run();

    // Accessors for subsystems
    class ProtocolManager *protocolManager() const;

private:
    void setupLogging();
    void setupNetwork();
    void setupProtocolSupport();
    void setupSystrayIntegration();
    void loadSavedSessions();
    void registerGlobalShortcuts();

    std::unique_ptr<MainWindow> m_mainWindow;
    std::unique_ptr<NotificationManager> m_notificationManager;
    std::unique_ptr<ThemeManager> m_themeManager;
    std::unique_ptr<ProtocolManager> m_protocolManager;
    std::unique_ptr<QNetworkAccessManager> m_networkManager;

    QString m_configDir;
    QString m_proxyUrl;
    bool m_debugLogging = false;
    bool m_crashReporting = true;
};

} // namespace pcp_chat
