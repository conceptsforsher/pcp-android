#pragma once

#include "../protocol/protocol_session.hpp"
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

class QNetworkAccessManager;

namespace pcp_chat {

struct LemmyCommunity {
    QString id;
    QString name;
    QString title;
    QString description;
    int subscribers = 0;
    int posts = 0;
    int comments = 0;
    bool subscribed = false;
    bool nsfw = false;
    QString icon;
    QString banner;
    QJsonObject raw;
};

struct LemmyPost {
    QString id;
    QString name;
    QString communityId;
    QString communityName;
    QString creatorId;
    QString creatorName;
    QString title;
    QString body;
    QString url;
    int score = 0;
    int upvotes = 0;
    int downvotes = 0;
    int comments = 0;
    bool nsfw = false;
    bool locked = false;
    bool stickied = false;
    bool read = false;
    QDateTime published;
    QDateTime updated;
    QJsonObject raw;
};

struct LemmyComment {
    QString id;
    QString postId;
    QString parentId;
    QString creatorId;
    QString creatorName;
    QString content;
    int score = 0;
    int upvotes = 0;
    int downvotes = 0;
    bool read = false;
    QDateTime published;
    QDateTime updated;
    QJsonObject raw;
};

struct LemmyNotification {
    QString id;
    QString type; // reply, mention, upvote, etc.
    QString postId;
    QString commentId;
    QString creatorId;
    QString creatorName;
    bool read = false;
    QDateTime published;
};

class LemmySession : public ProtocolSession
{
    Q_OBJECT

public:
    explicit LemmySession(QNetworkAccessManager *network, QObject *parent = nullptr);
    ~LemmySession() override;

    void setInstance(const QString &instance) { m_instance = instance; }
    void setAccessToken(const QString &token) { m_accessToken = token; }

    QString instance() const { return m_instance; }

    void connect() override;
    void disconnect() override;
    void reconnect() override;

    void sendMessage(const QString &roomId, const QString &body,
                     const QString &replyTo = "") override;
    void sendTyping(const QString &roomId, bool typing) override;
    void markRead(const QString &roomId) override;
    void joinRoom(const QString &roomId) override;
    void leaveRoom(const QString &roomId) override;

    // Lemmy-specific API
    void fetchCommunities(int page = 1, int limit = 50);
    void fetchPosts(const QString &communityId = "", int page = 1, int limit = 50,
                    const QString &sort = "Active");
    void fetchComments(const QString &postId, int page = 1, int limit = 50,
                       const QString &sort = "Hot");
    void fetchNotifications(int page = 1, int limit = 20);
    void votePost(const QString &postId, int score); // 1 = upvote, -1 = downvote, 0 = clear
    void subscribeCommunity(const QString &communityId, bool subscribe);
    void createPost(const QString &communityId, const QString &title,
                    const QString &body = "", const QString &url = "");
    void createComment(const QString &postId, const QString &content,
                       const QString &parentId = "");

signals:
    void communitiesLoaded(const QList<LemmyCommunity> &communities);
    void postsLoaded(const QList<LemmyPost> &posts);
    void commentsLoaded(const QList<LemmyComment> &comments);
    void notificationsLoaded(const QList<LemmyNotification> &notifications);
    void postCreated(const LemmyPost &post);
    void commentCreated(const LemmyComment &comment);
    void voteRegistered(const QString &postId, int newScore);

private:
    QNetworkRequest createRequest(const QString &path) const;
    void handleApiResponse(QNetworkReply *reply,
                           std::function<void(const QJsonObject &)> handler);
    void handleAuthError(int statusCode);

    QNetworkAccessManager *m_networkManager;
    QString m_instance;
    QString m_accessToken;
    QString m_username;
    QTimer m_pollTimer;
    int m_pollIntervalMs = 60000;
};

} // namespace pcp_chat
