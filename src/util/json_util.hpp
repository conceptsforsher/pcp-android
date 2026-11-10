#pragma once

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <optional>

namespace pcp_chat {
namespace util {

inline std::optional<QJsonObject> parseJson(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError || !doc.isObject())
        return std::nullopt;
    return doc.object();
}

inline std::optional<QJsonDocument> parseJsonDocument(const QByteArray &data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if (error.error != QJsonParseError::NoError)
        return std::nullopt;
    return doc;
}

inline QJsonObject safeObject(const QJsonValue &val)
{
    return val.isObject() ? val.toObject() : QJsonObject();
}

inline QJsonArray safeArray(const QJsonValue &val)
{
    return val.isArray() ? val.toArray() : QJsonArray();
}

inline QString safeString(const QJsonValue &val, const QString &defaultVal = "")
{
    return val.isString() ? val.toString() : defaultVal;
}

inline int safeInt(const QJsonValue &val, int defaultVal = 0)
{
    return val.isDouble() ? val.toInt() : defaultVal;
}

inline qint64 safeInt64(const QJsonValue &val, qint64 defaultVal = 0)
{
    return val.isDouble() ? (qint64)val.toDouble() : defaultVal;
}

inline bool safeBool(const QJsonValue &val, bool defaultVal = false)
{
    return val.isBool() ? val.toBool() : defaultVal;
}

inline QJsonObject mergeObjects(const QJsonObject &base, const QJsonObject &overlay)
{
    QJsonObject result = base;
    for (auto it = overlay.begin(); it != overlay.end(); ++it) {
        result[it.key()] = it.value();
    }
    return result;
}

QString canonicalJson(const QJsonObject &obj);
QJsonObject redactEvent(const QJsonObject &event);
QString eventHash(const QJsonObject &event);

} // namespace util
} // namespace pcp_chat
