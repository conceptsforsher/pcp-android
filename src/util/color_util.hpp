#pragma once

#include <QColor>
#include <QString>
#include <QHash>

namespace pcp_chat {
namespace util {

inline QColor colorFromUserId(const QString &userId)
{
    // Generate a consistent color from a user ID
    QByteArray hash = QCryptographicHash::hash(userId.toUtf8(), QCryptographicHash::Md5);
    int h = (uint8_t)hash[0] % 360;
    int s = 50 + (uint8_t)hash[1] % 30;
    int l = 40 + (uint8_t)hash[2] % 25;
    return QColor::fromHsl(h, s, l);
}

inline QString colorToHex(const QColor &color)
{
    return color.name();
}

inline QColor hexToColor(const QString &hex)
{
    return QColor(hex);
}

inline bool isDarkColor(const QColor &color)
{
    return color.lightness() < 128;
}

inline QColor textColorForBackground(const QColor &background)
{
    return isDarkColor(background) ? QColor("#ffffff") : QColor("#212121");
}

} // namespace util
} // namespace pcp_chat
