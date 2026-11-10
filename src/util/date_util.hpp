#pragma once

#include <QDateTime>
#include <QString>
#include <QDate>
#include <QTime>

namespace pcp_chat {
namespace util {

inline QString formatTimestamp(const QDateTime &dt)
{
    if (!dt.isValid()) return "";

    QDate today = QDate::currentDate();
    QDate date = dt.date();

    if (date == today)
        return dt.toString("HH:mm");
    else if (date == today.addDays(-1))
        return "Yesterday " + dt.toString("HH:mm");
    else if (date.year() == today.year())
        return dt.toString("MMM d, HH:mm");
    else
        return dt.toString("MMM d yyyy, HH:mm");
}

inline QString formatRelativeTime(const QDateTime &dt)
{
    if (!dt.isValid()) return "";

    qint64 secs = dt.secsTo(QDateTime::currentDateTime());

    if (secs < 0) return "just now";
    if (secs < 60) return QString("%1s ago").arg(secs);
    if (secs < 3600) return QString("%1m ago").arg(secs / 60);
    if (secs < 86400) return QString("%1h ago").arg(secs / 3600);
    if (secs < 604800) return QString("%1d ago").arg(secs / 86400);
    if (secs < 2592000) return QString("%1w ago").arg(secs / 604800);

    return formatTimestamp(dt);
}

inline QString formatDuration(qint64 milliseconds)
{
    int totalSeconds = milliseconds / 1000;
    int hours = totalSeconds / 3600;
    int minutes = (totalSeconds % 3600) / 60;
    int seconds = totalSeconds % 60;

    if (hours > 0)
        return QString("%1:%2:%3").arg(hours).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
    return QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0'));
}

inline QString formatDate(const QDate &date)
{
    if (!date.isValid()) return "";
    return date.toString("yyyy-MM-dd");
}

} // namespace util
} // namespace pcp_chat
