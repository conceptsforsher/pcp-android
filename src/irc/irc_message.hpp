#pragma once

#include <QString>
#include <QDateTime>

namespace pcp_chat {

struct IrcMessage {
    QString raw;
    QString prefix;
    QString command;
    QStringList params;
    QDateTime timestamp;

    QString sender() const {
        int excl = prefix.indexOf("!");
        return excl > 0 ? prefix.left(excl) : prefix;
    }

    QString userhost() const {
        int excl = prefix.indexOf("!");
        return excl > 0 ? prefix.mid(excl + 1) : "";
    }
};

} // namespace pcp_chat
