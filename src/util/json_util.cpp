#include "json_util.hpp"

#include <QJsonDocument>
#include <QCryptographicHash>
#include <QDebug>

namespace pcp_chat {
namespace util {

QString canonicalJson(const QJsonObject &obj)
{
    // Canonical JSON: sorted keys, no whitespace
    QJsonDocument doc(obj);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

QJsonObject redactEvent(const QJsonObject &event)
{
    QJsonObject result = event;

    // Remove content that should be stripped during redaction
    if (result.contains("unsigned")) {
        QJsonObject unsignedData = result["unsigned"].toObject();
        unsignedData.remove("age");
        unsignedData.remove("transaction_id");
        result["unsigned"] = unsignedData;
    }

    return result;
}

QString eventHash(const QJsonObject &event)
{
    // Hash of the redacted canonical JSON
    QJsonObject redacted = redactEvent(event);
    QString canonical = canonicalJson(redacted);
    return QString::fromUtf8(
        QCryptographicHash::hash(canonical.toUtf8(), QCryptographicHash::Sha256).toHex());
}

} // namespace util
} // namespace pcp_chat
