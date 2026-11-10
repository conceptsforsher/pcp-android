#pragma once

#include <QString>
#include <QMetaType>

namespace pcp_chat {

enum class ProtocolType {
    Matrix = 0,
    IRC = 1,
    Lemmy = 2,
    Unknown = 99
};

inline QString protocolTypeToString(ProtocolType type)
{
    switch (type) {
        case ProtocolType::Matrix: return "matrix";
        case ProtocolType::IRC: return "irc";
        case ProtocolType::Lemmy: return "lemmy";
        default: return "unknown";
    }
}

inline ProtocolType protocolTypeFromString(const QString &str)
{
    if (str == "matrix") return ProtocolType::Matrix;
    if (str == "irc") return ProtocolType::IRC;
    if (str == "lemmy") return ProtocolType::Lemmy;
    return ProtocolType::Unknown;
}

} // namespace pcp_chat

Q_DECLARE_METATYPE(pcp_chat::ProtocolType)
