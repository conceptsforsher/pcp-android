#pragma once

#include <QString>
#include <QStringList>
#include <QRegularExpression>

namespace pcp_chat {
namespace util {

inline QString ellipsize(const QString &text, int maxLen = 50)
{
    if (text.length() <= maxLen) return text;
    return text.left(maxLen - 3) + "...";
}

inline QString truncate(const QString &text, int maxLen)
{
    return text.left(maxLen);
}

inline QString stripHtml(const QString &html)
{
    QRegularExpression re("<[^>]*>");
    return html.replace(re, "");
}

inline QStringList splitLines(const QString &text)
{
    return text.split(QRegularExpression("[\r\n]+"), Qt::SkipEmptyParts);
}

inline bool isValidMatrixId(const QString &id)
{
    // @user:server or !room:server or #alias:server
    QRegularExpression re("^[@!#][a-zA-Z0-9._=/\-]+:[a-zA-Z0-9.-]+(?:\:[0-9]+)?$");
    return re.match(id).hasMatch();
}

inline QString extractServerName(const QString &matrixId)
{
    int colon = matrixId.lastIndexOf(':');
    return colon > 0 ? matrixId.mid(colon + 1) : "";
}

inline QString extractLocalPart(const QString &matrixId)
{
    int colon = matrixId.indexOf(':');
    return colon > 0 ? matrixId.mid(1, colon - 1) : matrixId;
}

inline QString sanitizeFilename(const QString &name)
{
    QString result = name;
    result.replace(QRegularExpression("[<>:"/\\\\|?*]"), "_");
    return result;
}

inline QString formatBytes(qint64 bytes)
{
    if (bytes < 1024) return QString("%1 B").arg(bytes);
    if (bytes < 1024 * 1024) return QString("%1 KB").arg(bytes / 1024.0, 0, 'f', 1);
    if (bytes < 1024 * 1024 * 1024) return QString("%1 MB").arg(bytes / (1024.0 * 1024.0), 0, 'f', 1);
    return QString("%1 GB").arg(bytes / (1024.0 * 1024.0 * 1024.0), 0, 'f', 2);
}

} // namespace util
} // namespace pcp_chat
