#pragma once

#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDateTime>
#include <QFileInfoList>

namespace FileUtil {

inline QString cacheDir()
{
    QString dir = QDir::homePath() + "/.cache/pcp-chat/";
    QDir().mkpath(dir);
    return dir;
}

inline QString dataDir()
{
    QString dir = QDir::homePath() + "/.local/share/pcp-chat/";
    QDir().mkpath(dir);
    return dir;
}

inline QString configDir()
{
    QString dir = QDir::homePath() + "/.config/pcp-chat/";
    QDir().mkpath(dir);
    return dir;
}

inline bool ensureDir(const QString &path)
{
    return QDir().mkpath(path);
}

inline QString uniqueFilename(const QString &prefix, const QString &ext)
{
    QString ts = QDateTime::currentDateTimeUtc().toString("yyyyMMddhhmmsszzz");
    return prefix + "_" + ts + "." + ext;
}

inline qint64 dirSize(const QString &path)
{
    qint64 total = 0;
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    for (const QFileInfo &info : entries) {
        if (info.isDir())
            total += dirSize(info.absoluteFilePath());
        else
            total += info.size();
    }
    return total;
}

inline bool removeDir(const QString &path)
{
    return QDir(path).removeRecursively();
}

inline QString humanReadableSize(qint64 bytes)
{
    if (bytes < 0)
        return "0 B";
    const char *units[] = {"B", "KB", "MB", "GB", "TB", "PB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);
    while (size >= 1024.0 && unitIndex < 5) {
        size /= 1024.0;
        unitIndex++;
    }
    if (unitIndex == 0)
        return QString::number(bytes) + " B";
    return QString::number(size, 'f', 1) + " " + units[unitIndex];
}

} // namespace FileUtil
