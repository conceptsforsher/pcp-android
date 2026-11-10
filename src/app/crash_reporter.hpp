#pragma once

#include <QString>
#include <QObject>
#include <functional>

namespace pcp_chat {

class CrashReporter : public QObject
{
    Q_OBJECT

public:
    static CrashReporter &instance();

    void initialize(const QString &configDir);
    void setUploadUrl(const QString &url);
    void setEnabled(bool enabled);

    void recordBreadcrumb(const QString &message);
    void recordUserAction(const QString &action);
    void recordNetworkRequest(const QString &url, int statusCode);

    void handleCrash(const QString &stackTrace, const QString &reason = "");

signals:
    void crashReportReady(const QString &reportPath);
    void uploadComplete(bool success);

private:
    explicit CrashReporter(QObject *parent = nullptr);
    ~CrashReporter() override;

    QString generateReport(const QString &stackTrace, const QString &reason);
    void saveReport(const QString &report);
    void uploadReport(const QString &report);

    QString m_configDir;
    QString m_uploadUrl;
    bool m_enabled = true;
    QStringList m_breadcrumbs;
    QStringList m_recentActions;
};

} // namespace pcp_chat
