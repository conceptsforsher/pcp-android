#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>

namespace pcp_chat {

enum class MatrixMsgType {
    Text, Notice, Emote, Image, Video, Audio, File,
    Location, Poll, Sticker, Encrypted, BadEncrypted,
    Unknown
};

struct MatrixMessageData {
    QString eventId;
    QString roomId;
    QString sender;
    QString senderName;
    QString senderAvatar;
    MatrixMsgType msgType = MatrixMsgType::Text;
    QString body;
    QString formattedBody;
    QString format = "org.matrix.custom.html";
    QDateTime originServerTs;
    QDateTime age;
    QString transactionId;
    QString relatesToEventId;
    QString relatesToType; // "m.annotation", "m.replace", "m.thread"
    bool isEncrypted = false;
    bool isVerified = false;
    QString decryptionError;
    QJsonObject rawContent;
    QJsonObject unsignedData;
    QJsonObject raw;

    bool isRedacted() const { return rawContent.contains("redacted_because"); }
    bool isEdit() const { return relatesToType == "m.replace"; }
    bool isReply() const {
        return relatesToType == "m.in_reply_to" ||
               rawContent.contains("m.relates_to") &&
               rawContent["m.relates_to"].toObject()["rel_type"] == "m.in_reply_to";
    }
};

} // namespace pcp_chat
