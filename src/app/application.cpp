#include "application.hpp"
#include "main_window.hpp"
#include "notification_manager.hpp"
#include "crash_reporter.hpp"
#include "log_collector.hpp"
#include "app_update_checker.hpp"
#include "../protocol/protocol_manager.hpp"
#include "../ui/themes/theme_manager.hpp"

#include <QDir>
#include <QStandardPaths>
#include <QNetworkProxy>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>
#include <QTimer>
#include <QSocketNotifier>
#include <QFile>
#include <QTextStream>
#include <QMutex>
#include <iostream>

namespace pcp_chat {

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
{
    setQuitOnLastWindowClosed(false);
}

Application::~Application() = default;

QString Application::configDirectory() const { return m_configDir; }
void Application::setConfigDirectory(const QString &path) { m_configDir = path; }
void Application::setDebugLogging(bool enabled) { m_debugLogging = enabled; }
void Application::setCrashReportingEnabled(bool enabled) { m_crashReporting = enabled; }
void Application::setProxyUrl(const QString &url) { m_proxyUrl = url; }

void Application::setLogFile(const QString &path)
{
    static QFile logFile;
    if (logFile.isOpen()) logFile.close();
    logFile.setFileName(path);
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
            QTextStream stream(&logFile);
            stream << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
                   << " [" << type << "] " << msg << "
";
            stream.flush();
        });
    }
}

bool Application::initialize()
{
    setupLogging();
    setupNetwork();
    setupProtocolSupport();

    // Initialize crash reporter
    if (m_crashReporting) {
        CrashReporter::instance().initialize(m_configDir);
    }

    // Initialize theme manager
    m_themeManager = std::make_unique<ThemeManager>();
    m_themeManager->loadTheme(m_configDir + "/theme.json");

    // Initialize notification manager
    m_notificationManager = std::make_unique<NotificationManager>();

    // Create main window
    m_mainWindow = std::make_unique<MainWindow>();
    m_mainWindow->setThemeManager(m_themeManager.get());
    m_mainWindow->setNotificationManager(m_notificationManager.get());
    m_mainWindow->setProtocolManager(m_protocolManager.get());
    m_mainWindow->initialize();

    // Setup system tray
    setupSystrayIntegration();

    // Load previously saved sessions
    loadSavedSessions();

    // Register keyboard shortcuts
    registerGlobalShortcuts();

    // Periodic update check (every 6 hours)
    auto *updateTimer = new QTimer(this);
    QObject::connect(updateTimer, &QTimer::timeout, this, []() {
        AppUpdateChecker::checkForUpdates();
    });
    updateTimer->start(6 * 60 * 60 * 1000);
    AppUpdateChecker::checkForUpdates();

    // Periodic log cleanup (every 24 hours)
    auto *logCleanupTimer = new QTimer(this);
    QObject::connect(logCleanupTimer, &QTimer::timeout, this, [this]() {
        LogCollector::cleanupOldLogs(m_configDir, 30);
    });
    logCleanupTimer->start(24 * 60 * 60 * 1000);

    return true;
}

int Application::run()
{
    m_mainWindow->show();
    return exec();
}

void Application::setupLogging()
{
    if (m_debugLogging) {
        QLoggingCategory::setFilterRules("*.debug=true\npcp.*=true");
    }

    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &ctx, const QString &msg) {
        static QMutex mutex;
        QMutexLocker lock(&mutex);

        const char *typeStr = "UNKNOWN";
        switch (type) {
            case QtDebugMsg:    typeStr = "DEBUG"; break;
            case QtInfoMsg:     typeStr = "INFO"; break;
            case QtWarningMsg:  typeStr = "WARN"; break;
            case QtCriticalMsg: typeStr = "CRITICAL"; break;
            case QtFatalMsg:    typeStr = "FATAL"; break;
        }

        QTextStream out(stderr);
        out << QDateTime::currentDateTime().toString(Qt::ISODateWithMs)
            << " [" << typeStr << "] "
            << (ctx.category ? ctx.category : "default") << ": "
            << msg << "
";
        out.flush();
    });
}

void Application::setupNetwork()
{
    m_networkManager = std::make_unique<QNetworkAccessManager>();

    if (!m_proxyUrl.isEmpty()) {
        QUrl proxyUrl(m_proxyUrl);
        QNetworkProxy proxy;
        if (proxyUrl.scheme() == "socks5" || proxyUrl.scheme() == "socks5h") {
            proxy.setType(QNetworkProxy::Socks5Proxy);
        } else {
            proxy.setType(QNetworkProxy::HttpProxy);
        }
        proxy.setHostName(proxyUrl.host());
        proxy.setPort(proxyUrl.port(1080));
        if (!proxyUrl.userName().isEmpty())
            proxy.setUser(proxyUrl.userName());
        if (!proxyUrl.password().isEmpty())
            proxy.setPassword(proxyUrl.password());
        QNetworkProxy::setApplicationProxy(proxy);
    }

    // Enable TLS 1.3
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setProtocol(QSsl::TlsV1_3OrLater);
    QSslConfiguration::setDefaultConfiguration(sslConfig);
}

void Application::setupProtocolSupport()
{
    m_protocolManager = std::make_unique<ProtocolManager>(m_networkManager.get());
    m_protocolManager->initialize();
}

void Application::setupSystrayIntegration()
{
    m_mainWindow->setupSystemTray(m_notificationManager.get());
}

void Application::loadSavedSessions()
{
    const QString sessionFile = m_configDir + "/sessions.json";
    QFile file(sessionFile);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isArray()) return;

    QJsonArray sessions = doc.array();
    for (const QJsonValue &val : sessions) {
        QJsonObject obj = val.toObject();
        QString type = obj["type"].toString();
        QString userId = obj["user_id"].toString();
        QString server = obj["server"].toString();
        QString token = obj["access_token"].toString();

        if (!token.isEmpty()) {
            m_protocolManager->restoreSession(type, userId, server, token);
        }
    }
}

void Application::registerGlobalShortcuts()
{
    // Ctrl+Shift+L — show log viewer
    // Ctrl+Shift+D — debug console
    // Ctrl+K — command palette (handled in MainWindow)
}

ProtocolManager *Application::protocolManager() const
{
    return m_protocolManager.get();
}

} // namespace pcp_chat
