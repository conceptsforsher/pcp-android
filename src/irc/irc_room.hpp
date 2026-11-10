#pragma once

#include <QString>
#include <QStringList>
#include <QDateTime>

namespace pcp_chat {

struct IrcRoomData {
    QString name;
    QString topic;
    QStringList users;
    QStringList ops;
    QStringList voiced;
    QStringList modes;
    QDateTime joinedAt;
    bool hasTopic = false;
};

} // namespace pcp_chat
