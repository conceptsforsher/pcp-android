#pragma once

#include "protocol_type.hpp"
#include "protocol_message.hpp"
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QVariantMap>

namespace pcp_chat {

enum class RoomType {
    Direct,
    Group,
    Space,
    Channel,
    Community,
    Unknown
};

struct ProtocolRoom {
    QString id;
    ProtocolType protocol = ProtocolType::Unknown;
    RoomType type = RoomType::Unknown;
    QString name;
    QString topic;
    QString avatarUrl;
    QString canonicalAlias;
    QStringList aliases;
    QStringList memberIds;
    int memberCount = 0;
    int unreadCount = 0;
    int highlightCount = 0;
    bool isEncrypted = false;
    bool isDirect = false;
    bool isFavorite = false;
    bool isLowPriority = false;
    bool hasUnread = false;
    bool isMuted = false;
    QDateTime lastActivity;
    QString lastMessagePreview;
    QString lastMessageSender;
    QVariantMap extra;

    QString displayName() const {
        return name.isEmpty() ? canonicalAlias : name;
    }

    bool isSpace() const { return type == RoomType::Space; }
};

} // namespace pcp_chat
