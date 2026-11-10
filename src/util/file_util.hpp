#pragma once

#include <QString>
#include <QFileInfo>
#include <QDir>

namespace pcp_chat {
namespace util {

inline bool fileExists(const QString &path)
{
    return QFileInfo::exists(path);
}

inline qint64 fileSize(const QString &path)
{
    return QFileInfo(path).size();
}

inline QString fileExtension(const QString &path)
{
    return QFileInfo(path).suffix().toLower();
}

inline QString fileName(const QString &path)
{
    return QFileInfo(path).fileName();
}

inline bool ensureDirectory(const QString &path)
{
    return QDir().mkpath(path);
}

inline QString joinPath(const QString &base, const QString &sub)
{
    return QDir(base).filePath(sub);
}

inline QString cacheDirectory(const QString &appDir)
{
    return QDir(appDir).filePath("cache");
}

inline QString downloadsDirectory(const QString &appDir)
{
    return QDir(appDir).filePath("downloads");
}

inline QString thumbnailsDirectory(const QString &appDir)
{
    return QDir(appDir).filePath("thumbnails");
}

} // namespace util
} // namespace pcp_chat
