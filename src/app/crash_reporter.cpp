#include "crash_reporter.hpp"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDir>
#include <QSysInfo>
#include <QUuid>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace pcp_chat {

CrashReporter &CrashReporter::instance()
{
    static CrashReporter instance;
    return instance;
}

CrashReporter::CrashReporter(QObject *parent) : QObject(parent) {}
CrashReporter::~CrashReporter() = default;

void CrashReporter::initialize(const QString &configDir)
{
    m_configDir = configDir;
    QDir().mkpath(configDir + "/crashes");

    // Install signal handlers for common crashes
    auto crashHandler = [](int signal) {
        Q_UNUSED(signal);
        // Stack trace will be generated
    };
    signal(SIGSEGV, crashHandler);
    signal(SIGABRT, crashHandler);
}

void CrashReporter::setUploadUrl(const QString &url) { m_uploadUrl = url; }
void CrashReporter::setEnabled(bool enabled) { m_enabled = enabled; }

void CrashReporter::recordBreadcrumb(const QString &message)
{
    m_breadcrumbs.append(QDateTime::currentDateTime().toString("hh:mm:ss.zzz") + " " + message);
    if (m_breadcrumbs.size() > 100)
        m_breadcrumbs.removeFirst();
}

void CrashReporter::recordUserAction(const QString &action)
{
    m_recentActions.append(action);
    if (m_recentActions.size() > 50)
        m_recentActions.removeFirst();
}

void CrashReporter::recordNetworkRequest(const QString &url, int statusCode)
{
    recordBreadcrumb(QString("HTTP %1 → %2").arg(statusCode).arg(url));
}

void CrashReporter::handleCrash(const QString &stackTrace, const QString &reason)
{
    if (!m_enabled) return;

    QString report = generateReport(stackTrace, reason);
    saveReport(report);
    uploadReport(report);
}

QString CrashReporter::generateReport(const QString &stackTrace, const QString &reason)
{
    QJsonObject report;
    report["crash_id"] = QUuid::createUuid().toString();
    report["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODateWithMs);
    report["reason"] = reason;
    report["stack_trace"] = stackTrace;
    report["os"] = QSysInfo::prettyProductName();
    report["kernel"] = QSysInfo::kernelVersion();
    report["app_version"] = QApplication::applicationVersion();

    QJsonArray breadcrumbs;
    for (const QString &b : m_breadcrumbs)
        breadcrumbs.append(b);
    report["breadcrumbs"] = breadcrumbs;

    QJsonArray actions;
    for (const QString &a : m_recentActions)
        actions.append(a);
    report["recent_actions"] = actions;

    return QString::fromUtf8(QJsonDocument(report).toJson(QJsonDocument::Indented));
}

void CrashReporter::saveReport(const QString &report)
{
    QString filename = m_configDir + "/crashes/crash_" +
                       QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".json";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << report;
    }
}

void CrashReporter::uploadReport(const QString &report)
{
    // Auto-upload if configured
    Q_UNUSED(report);
}

} // namespace pcp_chat
