#include "lemmy_session.hpp"
#include "../protocol/protocol_room.hpp"

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

LemmySession::LemmySession(QNetworkAccessManager *network, QObject *parent)
    : ProtocolSession(parent)
    , m_networkManager(network)
{
    m_protocolType = ProtocolType::Lemmy;
}

LemmySession::~LemmySession()
{
    m_pollTimer.stop();
}

void LemmySession::connect()
{
    if (m_instance.isEmpty()) {
        emit connectionError("No Lemmy instance configured");
        return;
    }

    // Verify connectivity by fetching site info
    auto *reply = m_networkManager->get(createRequest("/api/v3/site"));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QJsonObject siteView = obj["site_view"].toObject();
            QJsonObject site = siteView["site"].toObject();
            m_username = site["name"].toString();

            setConnected(true);

            // Start periodic polling
            QObject::connect(&m_pollTimer, &QTimer::timeout, this, [this]() {
                fetchNotifications(1, 20);
            });
            m_pollTimer.start(m_pollIntervalMs);

            fetchCommunities();
        } else {
            handleAuthError(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        }
        reply->deleteLater();
    });
}

void LemmySession::disconnect()
{
    m_pollTimer.stop();
    setConnected(false);
}

void LemmySession::reconnect()
{
    disconnect();
    QTimer::singleShot(2000, this, &LemmySession::connect);
}

void LemmySession::sendMessage(const QString &roomId, const QString &body, const QString &replyTo)
{
    // In Lemmy, "rooms" are communities and messages are comments
    createComment(roomId, body, replyTo);
}

void LemmySession::sendTyping(const QString &roomId, bool typing)
{
    Q_UNUSED(roomId); Q_UNUSED(typing);
}

void LemmySession::markRead(const QString &roomId) { Q_UNUSED(roomId); }

void LemmySession::joinRoom(const QString &roomId)
{
    subscribeCommunity(roomId, true);
}

void LemmySession::leaveRoom(const QString &roomId)
{
    subscribeCommunity(roomId, false);
}

QNetworkRequest LemmySession::createRequest(const QString &path) const
{
    QUrl url(m_instance + path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    if (!m_accessToken.isEmpty()) {
        request.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());
    }

    return request;
}

void LemmySession::fetchCommunities(int page, int limit)
{
    QUrl url(m_instance + "/api/v3/community/list");
    QUrlQuery query;
    query.addQueryItem("type_", "Subscribed");
    query.addQueryItem("sort", "TopMonth");
    query.addQueryItem("page", QString::number(page));
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    auto *reply = m_networkManager->get(QNetworkRequest(url));
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QList<LemmyCommunity> communities;
        QJsonArray arr = obj["communities"].toArray();

        for (const QJsonValue &val : arr) {
            QJsonObject cv = val.toObject()["community"].toObject();
            QJsonObject counts = val.toObject()["counts"].toObject();

            LemmyCommunity c;
            c.id = QString::number(cv["id"].toInt());
            c.name = cv["name"].toString();
            c.title = cv["title"].toString();
            c.description = cv["description"].toString();
            c.subscribers = counts["subscribers"].toInt();
            c.posts = counts["posts"].toInt();
            c.comments = counts["comments"].toInt();
            c.nsfw = cv["nsfw"].toBool();
            c.subscribed = val.toObject()["subscribed"].toString() == "Subscribed";
            c.raw = cv;
            communities.append(c);

            // Add as ProtocolRoom
            ProtocolRoom room;
            room.id = "lemmy_" + c.id;
            room.protocol = ProtocolType::Lemmy;
            room.type = RoomType::Community;
            room.name = c.title.isEmpty() ? c.name : c.title;
            room.topic = c.description;
            room.avatarUrl = c.icon;
            room.memberCount = c.subscribers;
            room.extra["community_id"] = c.id;
            room.extra["community_name"] = c.name;

            if (hasRoom(room.id))
                updateRoom(room);
            else
                addRoom(room);
        }

        emit communitiesLoaded(communities);
    });
}

void LemmySession::fetchPosts(const QString &communityId, int page, int limit,
                               const QString &sort)
{
    QUrl url(m_instance + "/api/v3/post/list");
    QUrlQuery query;

    if (!communityId.isEmpty())
        query.addQueryItem("community_id", communityId);
    query.addQueryItem("sort", sort);
    query.addQueryItem("page", QString::number(page));
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    auto *reply = m_networkManager->get(QNetworkRequest(url));
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QList<LemmyPost> posts;
        QJsonArray arr = obj["posts"].toArray();

        for (const QJsonValue &val : arr) {
            QJsonObject pv = val.toObject();
            QJsonObject post = pv["post"].toObject();
            QJsonObject counts = pv["counts"].toObject();
            QJsonObject creator = pv["creator"].toObject();
            QJsonObject community = pv["community"].toObject();

            LemmyPost p;
            p.id = QString::number(post["id"].toInt());
            p.name = post["name"].toString();
            p.communityId = QString::number(community["id"].toInt());
            p.communityName = community["name"].toString();
            p.creatorId = QString::number(creator["id"].toInt());
            p.creatorName = creator["name"].toString();
            p.title = post["name"].toString();
            p.body = post["body"].toString();
            p.url = post["url"].toString();
            p.score = counts["score"].toInt();
            p.upvotes = counts["upvotes"].toInt();
            p.downvotes = counts["downvotes"].toInt();
            p.comments = counts["comments"].toInt();
            p.nsfw = post["nsfw"].toBool();
            p.locked = post["locked"].toBool();
            p.stickied = post["stickied"].toBool();
            p.read = pv["read"].toBool();
            p.published = QDateTime::fromString(post["published"].toString(), Qt::ISODate);
            p.raw = post;
            posts.append(p);
        }

        emit postsLoaded(posts);
    });
}

void LemmySession::fetchComments(const QString &postId, int page, int limit,
                                  const QString &sort)
{
    QUrl url(m_instance + "/api/v3/comment/list");
    QUrlQuery query;
    query.addQueryItem("post_id", postId);
    query.addQueryItem("sort", sort);
    query.addQueryItem("page", QString::number(page));
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    auto *reply = m_networkManager->get(QNetworkRequest(url));
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QList<LemmyComment> comments;
        QJsonArray arr = obj["comments"].toArray();

        for (const QJsonValue &val : arr) {
            QJsonObject cv = val.toObject();
            QJsonObject comment = cv["comment"].toObject();
            QJsonObject counts = cv["counts"].toObject();
            QJsonObject creator = cv["creator"].toObject();

            LemmyComment c;
            c.id = QString::number(comment["id"].toInt());
            c.postId = QString::number(comment["post_id"].toInt());
            c.creatorId = QString::number(creator["id"].toInt());
            c.creatorName = creator["name"].toString();
            c.content = comment["content"].toString();
            c.score = counts["score"].toInt();
            c.upvotes = counts["upvotes"].toInt();
            c.downvotes = counts["downvotes"].toInt();
            c.published = QDateTime::fromString(comment["published"].toString(), Qt::ISODate);
            c.raw = comment;
            comments.append(c);
        }

        emit commentsLoaded(comments);
    });
}

void LemmySession::fetchNotifications(int page, int limit)
{
    if (m_accessToken.isEmpty()) return;

    QUrl url(m_instance + "/api/v3/user/mention");
    QUrlQuery query;
    query.addQueryItem("sort", "New");
    query.addQueryItem("unread_only", "true");
    query.addQueryItem("page", QString::number(page));
    query.addQueryItem("limit", QString::number(limit));
    url.setQuery(query);

    auto *reply = m_networkManager->get(createRequest(url.path() + "?" + url.query()));
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QList<LemmyNotification> notifications;
        QJsonArray arr = obj["mentions"].toArray();

        for (const QJsonValue &val : arr) {
            QJsonObject nv = val.toObject();

            LemmyNotification n;
            n.id = QString::number(nv["id"].toInt());
            n.type = "mention";
            n.commentId = QString::number(nv["comment"].toObject()["id"].toInt());
            n.postId = QString::number(nv["post"].toObject()["id"].toInt());
            n.creatorId = QString::number(nv["creator"].toObject()["id"].toInt());
            n.creatorName = nv["creator"].toObject()["name"].toString();
            n.read = nv["read"].toBool();
            n.published = QDateTime::fromString(nv["published"].toString(), Qt::ISODate);
            notifications.append(n);
        }

        emit notificationsLoaded(notifications);
    });
}

void LemmySession::votePost(const QString &postId, int score)
{
    QUrl url(m_instance + "/api/v3/post/like");
    QJsonObject body;
    body["post_id"] = postId.toInt();
    body["score"] = score;

    QNetworkRequest req = createRequest("/api/v3/post/like");
    auto *reply = m_networkManager->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply, postId]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            emit voteRegistered(postId, doc.object()["post"].toObject()["counts"]
                                           .toObject()["score"].toInt());
        }
        reply->deleteLater();
    });
}

void LemmySession::subscribeCommunity(const QString &communityId, bool subscribe)
{
    QUrl url(m_instance + "/api/v3/community/follow");
    QJsonObject body;
    body["community_id"] = communityId.toInt();
    body["follow"] = subscribe;

    QNetworkRequest req = createRequest("/api/v3/community/follow");
    auto *reply = m_networkManager->post(req, QJsonDocument(body).toJson());
    reply->deleteLater();
}

void LemmySession::createPost(const QString &communityId, const QString &title,
                                const QString &body, const QString &url)
{
    QNetworkRequest req = createRequest("/api/v3/post");
    QJsonObject postBody;
    postBody["community_id"] = communityId.toInt();
    postBody["name"] = title;
    if (!body.isEmpty()) postBody["body"] = body;
    if (!url.isEmpty()) postBody["url"] = url;

    auto *reply = m_networkManager->post(req, QJsonDocument(postBody).toJson());
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QJsonObject pv = obj["post_view"].toObject();
        LemmyPost p;
        p.id = QString::number(pv["post"].toObject()["id"].toInt());
        p.title = pv["post"].toObject()["name"].toString();
        emit postCreated(p);
    });
}

void LemmySession::createComment(const QString &postId, const QString &content,
                                   const QString &parentId)
{
    QNetworkRequest req = createRequest("/api/v3/comment");
    QJsonObject commentBody;
    commentBody["post_id"] = postId.toInt();
    commentBody["content"] = content;
    if (!parentId.isEmpty())
        commentBody["parent_id"] = parentId.toInt();

    auto *reply = m_networkManager->post(req, QJsonDocument(commentBody).toJson());
    handleApiResponse(reply, [this](const QJsonObject &obj) {
        QJsonObject cv = obj["comment_view"].toObject();
        LemmyComment c;
        c.id = QString::number(cv["comment"].toObject()["id"].toInt());
        c.content = cv["comment"].toObject()["content"].toString();
        emit commentCreated(c);
    });
}

void LemmySession::handleApiResponse(QNetworkReply *reply,
                                       std::function<void(const QJsonObject &)> handler)
{
    connect(reply, &QNetworkReply::finished, this, [this, reply, handler]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            if (doc.isObject()) {
                handler(doc.object());
            }
        } else {
            handleAuthError(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
        }
        reply->deleteLater();
    });
}

void LemmySession::handleAuthError(int statusCode)
{
    if (statusCode == 401 || statusCode == 403) {
        emit connectionError("Authentication failed");
        disconnect();
    } else if (statusCode == 404) {
        emit connectionError("Instance not found: " + m_instance);
    }
}

} // namespace pcp_chat
