#pragma once

#include <QString>
#include <QUrl>
#include <QNetworkRequest>

namespace pcp_chat {
namespace util {

inline bool isValidUrl(const QString &url)
{
    QUrl parsed(url);
    return parsed.isValid() && !parsed.scheme().isEmpty() && !parsed.host().isEmpty();
}

inline QString stripQueryAndFragment(const QString &url)
{
    QUrl parsed(url);
    parsed.setQuery(QString());
    parsed.setFragment(QString());
    return parsed.toString();
}

inline QString extractFilenameFromUrl(const QString &url)
{
    QUrl parsed(url);
    QString path = parsed.path();
    int lastSlash = path.lastIndexOf('/');
    return lastSlash >= 0 ? path.mid(lastSlash + 1) : "file";
}

inline bool isMxcUrl(const QString &url)
{
    return url.startsWith("mxc://");
}

inline QPair<QString, QString> parseMxcUrl(const QString &url)
{
    // Returns {serverName, mediaId}
    QString clean = url.mid(6); // Remove "mxc://"
    int slash = clean.indexOf("/");
    if (slash > 0)
        return {clean.left(slash), clean.mid(slash + 1)};
    return {clean, ""};
}

inline QString mxcToHttp(const QString &mxcUrl, const QString &homeserver)
{
    auto [server, mediaId] = parseMxcUrl(mxcUrl);
    return QString("%1/_matrix/media/v3/download/%2/%3").arg(homeserver, server, mediaId);
}

} // namespace util
} // namespace pcp_chat
