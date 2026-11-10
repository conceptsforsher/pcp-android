#pragma once

#include "protocol_type.hpp"
#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QStringList>
#include <QVariant>

namespace pcp_chat {

enum class MessageType {
    Text,
    Image,
    Video,
    Audio,
    File,
    Location,
    Notice,
    Emote,
    Poll,
    Sticker,
    System,
    Unknown
};

enum class MessageStatus {
    Sending,
    Sent,
    Delivered,
    Read,
    Failed,
    Unknown
};

struct ProtocolMessage {
    QString id;
    ProtocolType protocol = ProtocolType::Unknown;
    MessageType type = MessageType::Text;
    QString roomId;
    QString senderId;
    QString senderName;
    QString senderAvatarUrl;
    QString body;
    QString formattedBody;
    QDateTime timestamp;
    QDateTime editedTimestamp;
    MessageStatus status = MessageStatus::Unknown;
    QString replyToId;
    QString editOfId;
    QString threadRootId;
    QStringList mentions;
    QJsonObject rawEvent;
    QVariantMap extra;

    bool isEncrypted() const {
        return extra.contains("encrypted") && extra["encrypted"].toBool();
    }

    bool isEdited() const {
        return editedTimestamp.isValid();
    }

    bool isReply() const {
        return !replyToId.isEmpty();
    }

    bool isInThread() const {
        return !threadRootId.isEmpty();
    }

    QString displayName() const {
        return senderName.isEmpty() ? senderId : senderName;
    }

    static MessageType typeFromString(const QString &str);
    static QString typeToString(MessageType type);
    static MessageStatus statusFromString(const QString &str);
};

} // namespace pcp_chat
