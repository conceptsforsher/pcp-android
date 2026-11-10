#pragma once

#include <QString>
#include <QStringList>
#include <QDateTime>

namespace pcp_chat {

class LogCollector
{
public:
    static QStringList collectRecentLogs(int maxLines = 1000);
    static QString collectSessionLog(const QString &sessionId);
    static void cleanupOldLogs(const QString &configDir, int keepDays = 30);
    static QString formatLogForExport(const QStringList &logs, const QDateTime &from, const QDateTime &to);
    static void exportLogs(const QString &path, const QStringList &logs);

    static QString logDirectory(const QString &configDir);
    static QString currentLogPath(const QString &configDir);
};

} // namespace pcp_chat
