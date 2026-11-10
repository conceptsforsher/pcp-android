#pragma once

#include <QString>
#include <QStringList>
#include <QDateTime>
#include <QJsonObject>

namespace pcp_chat {

struct MatrixRoomData {
    QString roomId;
    QString name;
    QString topic;
    QString avatarUrl;
    QString canonicalAlias;
    QStringList aliases;
    QStringList joinedMembers;
    int joinedMemberCount = 0;
    bool isEncrypted = false;
    bool isDirect = false;
    bool isSpace = false;
    bool isFederated = true;
    int unreadCount = 0;
    int highlightCount = 0;
    int notificationCount = 0;
    QString lastMessagePreview;
    QString lastMessageSender;
    QDateTime lastActivity;
    QJsonObject roomCreateEvent;
    QJsonObject joinRules;
    QJsonObject powerLevels;
    QJsonObject tombstone;
    QString predecessorRoomId;
    QString successorRoomId;
    QString roomType;

    QString displayName() const {
        if (!name.isEmpty()) return name;
        if (!canonicalAlias.isEmpty()) return canonicalAlias;
        // Generate name from members for DMs
        if (isDirect && joinedMembers.size() <= 2) {
            return joinedMembers.value(0, roomId);
        }
        return roomId;
    }
};

} // namespace pcp_chat
