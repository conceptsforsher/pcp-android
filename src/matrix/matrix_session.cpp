#include "matrix_session.hpp"
#include "../protocol/protocol_room.hpp"
#include "../protocol/protocol_message.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
#include <QUrlQuery>
#include <QDebug>
#include <QUuid>

namespace pcp_chat {

MatrixSession::MatrixSession(QNetworkAccessManager *network, QObject *parent)
    : ProtocolSession(parent)
    , m_networkManager(network)
    , m_deviceId(QUuid::createUuid().toString().mid(1, 36))
{
    m_protocolType = ProtocolType::Matrix;
}

MatrixSession::~MatrixSession()
{
    m_syncTimer.stop();
}

void MatrixSession::setHomeserver(const QString &url) { m_homeserver = url; }
void MatrixSession::setAccessToken(const QString &token) { m_accessToken = token; }
void MatrixSession::setDeviceId(const QString &id) { m_deviceId = id; }

void MatrixSession::connect()
{
    if (m_homeserver.isEmpty()) {
        emit connectionError("No homeserver configured");
        return;
    }

    if (m_accessToken.isEmpty()) {
        emit connectionError("No access token");
        return;
    }

    // Resolve .well-known
    performServerDiscovery();
}

void MatrixSession::disconnect()
{
    m_syncTimer.stop();
    setConnected(false);
}

void MatrixSession::reconnect()
{
    disconnect();
    connect();
}

void MatrixSession::performServerDiscovery()
{
    // Try well-known first
    QUrl wellKnownUrl(m_homeserver);
    wellKnownUrl.setPath("/.well-known/matrix/client");

    QNetworkRequest request(wellKnownUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        QString baseUrl = m_homeserver;
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            if (obj.contains("m.homeserver")) {
                baseUrl = obj["m.homeserver"].toObject()["base_url"].toString();
                m_homeserver = baseUrl;
                emit homeserverResolved(baseUrl);
            }
        }
        reply->deleteLater();

        // Proceed with sync
        startSyncLoop();
    });
}

void MatrixSession::performLogin() {}

void MatrixSession::startSyncLoop()
{
    QObject::connect(&m_syncTimer, &QTimer::timeout, this, [this]() {
        sync();
    });
    m_syncTimer.setInterval(30000);
    sync(); // Initial sync
}

void MatrixSession::sync()
{
    QUrl url(m_homeserver + "/_matrix/client/v3/sync");
    QUrlQuery query;
    if (!m_nextBatch.isEmpty())
        query.addQueryItem("since", m_nextBatch);
    query.addQueryItem("timeout", QString::number(m_syncTimeoutSecs * 1000));
    query.addQueryItem("full_state", m_initialSyncDone ? "false" : "true");
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            processSyncResponse(reply->readAll());
            if (!m_connected) {
                setConnected(true);
            }
        } else {
            QString errorStr = reply->errorString();
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 401) {
                emit connectionError("Authentication failed — token expired");
                disconnect();
            } else {
                emit syncError(errorStr);
            }
        }
        reply->deleteLater();

        if (!m_syncTimer.isActive())
            m_syncTimer.start();
    });
}

void MatrixSession::processSyncResponse(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    m_nextBatch = obj["next_batch"].toString();
    m_initialSyncDone = true;

    if (obj.contains("rooms")) {
        processRoomsData(obj["rooms"].toObject());
    }

    emit syncCompleted(m_nextBatch);
}

void MatrixSession::processRoomsData(const QJsonObject &rooms)
{
    // Joined rooms
    if (rooms.contains("join")) {
        QJsonObject joined = rooms["join"].toObject();
        for (auto it = joined.begin(); it != joined.end(); ++it) {
            ProtocolRoom room;
            room.id = it.key();
            room.protocol = ProtocolType::Matrix;

            QJsonObject roomData = it.value().toObject();
            QJsonObject state = roomData["state"].toObject();

            if (state.contains("events")) {
                QJsonArray stateEvents = state["events"].toArray();
                for (const auto &ev : stateEvents) {
                    QJsonObject event = ev.toObject();
                    QString type = event["type"].toString();
                    QJsonObject content = event["content"].toObject();

                    if (type == "m.room.name")
                        room.name = content["name"].toString();
                    else if (type == "m.room.topic")
                        room.topic = content["topic"].toString();
                    else if (type == "m.room.canonical_alias")
                        room.canonicalAlias = content["alias"].toString();
                    else if (type == "m.room.encryption")
                        room.isEncrypted = true;
                    else if (type == "m.room.avatar")
                        room.avatarUrl = content["url"].toString();
                }
            }

            QJsonObject summary = roomData["summary"].toObject();
            room.unreadCount = summary["notification_count"].toInt();
            room.highlightCount = summary["highlight_count"].toInt();

            QJsonObject timeline = roomData["timeline"].toObject();
            if (timeline.contains("events")) {
                QJsonArray events = timeline["events"].toArray();
                if (!events.isEmpty()) {
                    QJsonObject lastEvent = events.last().toObject();
                    QJsonObject content = lastEvent["content"].toObject();
                    room.lastMessagePreview = content["body"].toString();
                    room.lastMessageSender = lastEvent["sender"].toString();
                    room.lastActivity = QDateTime::fromMSecsSinceEpoch(
                        lastEvent["origin_server_ts"].toDouble());
                }
            }

            if (hasRoom(room.id))
                updateRoom(room);
            else
                addRoom(room);
        }
    }
}

void MatrixSession::sendMessage(const QString &roomId, const QString &body,
                                  const QString &replyTo)
{
    QString txnId = QUuid::createUuid().toString().mid(1, 36);
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/send/m.room.message/" + txnId);

    QJsonObject content;
    content["msgtype"] = "m.text";
    content["body"] = body;

    if (!replyTo.isEmpty()) {
        QJsonObject relatesTo;
        QJsonObject inReplyTo;
        inReplyTo["event_id"] = replyTo;
        relatesTo["m.in_reply_to"] = inReplyTo;
        content["m.relates_to"] = relatesTo;
    }

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->put(request, QJsonDocument(content).toJson());
    connect(reply, &QNetworkReply::finished, this, [reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Failed to send message:" << reply->errorString();
        }
        reply->deleteLater();
    });
}

void MatrixSession::sendTyping(const QString &roomId, bool typing)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/typing/" + m_userId);
    QJsonObject body;
    body["typing"] = typing;
    body["timeout"] = typing ? 30000 : 0;

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->put(request, QJsonDocument(body).toJson());
    reply->deleteLater(); // Fire and forget
}

void MatrixSession::markRead(const QString &roomId)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/read_markers");
    QJsonObject body;
    body["m.fully_read"] = m_nextBatch;
    body["m.read"] = m_nextBatch;

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->post(request, QJsonDocument(body).toJson());
    reply->deleteLater();
}

void MatrixSession::joinRoom(const QString &roomId)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/join/" + roomId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body;
    body["reason"] = "User joined via PCP";

    auto *reply = m_networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, roomId, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString joinedRoomId = doc.object()["room_id"].toString();
            sync(); // Refresh room list
        }
        reply->deleteLater();
    });
}

void MatrixSession::leaveRoom(const QString &roomId)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/leave");

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->post(request, QJsonDocument().toJson());
    connect(reply, &QNetworkReply::finished, this, [this, roomId, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            removeRoom(roomId);
        }
        reply->deleteLater();
    });
}

void MatrixSession::sendReadReceipt(const QString &roomId, const QString &eventId)
{
    sendReceipt(roomId, eventId, "m.read");
}

void MatrixSession::sendReadMarkers(const QString &roomId, const QString &fullyRead,
                                       const QString &read)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/read_markers");
    QJsonObject body;
    if (!fullyRead.isEmpty()) body["m.fully_read"] = fullyRead;
    if (!read.isEmpty()) body["m.read"] = read;

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->post(request, QJsonDocument(body).toJson());
    reply->deleteLater();
}

void MatrixSession::sendReceipt(const QString &roomId, const QString &eventId,
                                  const QString &receiptType)
{
    QUrl url(m_homeserver + "/_matrix/client/v3/rooms/" + roomId + "/receipt/" + receiptType + "/" + eventId);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto *reply = m_networkManager->post(request, QJsonDocument().toJson());
    reply->deleteLater();
}

} // namespace pcp_chat
