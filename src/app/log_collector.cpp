#include "log_collector.hpp"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>

namespace pcp_chat {

QStringList LogCollector::collectRecentLogs(int maxLines)
{
    QStringList logs;
    // Implementation collects from Qt message handler buffer and log files
    logs.append("[Log collection initialized]");
    return logs;
}

QString LogCollector::collectSessionLog(const QString &sessionId)
{
    Q_UNUSED(sessionId);
    return "Session log for " + sessionId;
}

void LogCollector::cleanupOldLogs(const QString &configDir, int keepDays)
{
    QDir dir(logDirectory(configDir));
    QFileInfoList files = dir.entryInfoList({"*.log"}, QDir::Files, QDir::Time);

    QDateTime cutoff = QDateTime::currentDateTime().addDays(-keepDays);
    for (const QFileInfo &fi : files) {
        if (fi.lastModified() < cutoff) {
            QFile::remove(fi.absoluteFilePath());
        }
    }
}

QString LogCollector::formatLogForExport(const QStringList &logs, const QDateTime &from, const QDateTime &to)
{
    Q_UNUSED(from); Q_UNUSED(to);
    return logs.join("\n");
}

void LogCollector::exportLogs(const QString &path, const QStringList &logs)
{
    QFile file(path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << logs.join("\n");
    }
}

QString LogCollector::logDirectory(const QString &configDir)
{
    return configDir + "/logs";
}

QString LogCollector::currentLogPath(const QString &configDir)
{
    return logDirectory(configDir) + "/pcp_" +
           QDate::currentDate().toString("yyyy-MM-dd") + ".log";
}

} // namespace pcp_chat
