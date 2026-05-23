#include "progressive/lemmy_api.hpp"
#include "progressive/http_client.hpp"
#include <cstdio>

namespace progressive {

// ==== Internal state ====

static std::string g_lemmyInstance; // e.g. "https://lemmy.ml"

void setLemmyInstance(const std::string& instanceUrl) {
    g_lemmyInstance = instanceUrl;
    while (!g_lemmyInstance.empty() && g_lemmyInstance.back() == '/') {
        g_lemmyInstance.pop_back();
    }
}

const std::string& getLemmyInstance() {
    return g_lemmyInstance;
}

std::string lemmyBuildUrl(const std::string& path) {
    std::string url = g_lemmyInstance;
    if (url.empty()) return "";
    if (!path.empty() && path[0] != '/') url += '/';
    url += path;
    return url;
}

// ==== Internal helpers ====

namespace {

std::string urlEncode(const std::string& value) {
    std::string result;
    result.reserve(value.size() * 3);
    for (unsigned char c : value) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
            result += static_cast<char>(c);
        } else if (c == ' ') {
            result += "%20";
        } else {
            char hex[4];
            std::snprintf(hex, sizeof(hex), "%%%02X", c);
            result += hex;
        }
    }
    return result;
}

// Build auth query param suffix
std::string authQuery(const std::string& auth) {
    if (auth.empty()) return "";
    return "&auth=" + urlEncode(auth);
}

// HTTP GET with auth via query param
std::string getJson(const std::string& path, const std::string& auth, int timeoutMs = 30000) {
    HttpRequest req{"GET", lemmyBuildUrl(path + authQuery(auth)), "", {}, timeoutMs};
    return httpExecute(req).body;
}

// HTTP POST with optional auth (Bearer header)
std::string postJson(const std::string& path, const std::string& jsonBody,
                     const std::string& auth, int timeoutMs = 30000) {
    std::unordered_map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    if (!auth.empty()) headers["Authorization"] = "Bearer " + auth;
    return httpPost(lemmyBuildUrl(path), jsonBody, headers, timeoutMs).body;
}

// HTTP PUT with optional auth (Bearer header) — uses raw execute since httpPut lacks headers param
std::string putJson(const std::string& path, const std::string& jsonBody,
                    const std::string& auth, int timeoutMs = 30000) {
    std::unordered_map<std::string, std::string> headers;
    headers["Content-Type"] = "application/json";
    if (!auth.empty()) headers["Authorization"] = "Bearer " + auth;
    HttpRequest req{"PUT", lemmyBuildUrl(path), jsonBody, headers, timeoutMs};
    return httpExecute(req).body;
}

} // anonymous namespace

// ==== Public utility ====

std::string lemmyBuildJson(const std::vector<std::pair<std::string, std::string>>& fields) {
    std::string json = "{";
    for (size_t i = 0; i < fields.size(); ++i) {
        if (i > 0) json += ",";
        json += "\"" + fields[i].first + "\":" + fields[i].second;
    }
    json += "}";
    return json;
}

std::string lemmyBuildQuery(const std::vector<std::pair<std::string, std::string>>& params) {
    std::string query;
    for (size_t i = 0; i < params.size(); ++i) {
        if (i > 0) query += "&";
        query += params[i].first + "=" + urlEncode(params[i].second);
    }
    return query;
}

// ==== Site ====

std::string lemmyGetSite(const std::string& auth) {
    return getJson("/api/v3/site", auth);
}

// ==== Auth ====

std::string lemmyLogin(const std::string& username, const std::string& password,
                       const std::string& totp2faToken) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"username_or_email", "\"" + username + "\""},
        {"password", "\"" + password + "\""},
    };
    if (!totp2faToken.empty()) {
        fields.push_back({"totp_2fa_token", "\"" + totp2faToken + "\""});
    }
    return postJson("/api/v3/user/login", lemmyBuildJson(fields), "", 15000);
}

std::string lemmyRegister(const std::string& username, const std::string& password,
                          const std::string& email, const std::string& captchaUuid,
                          const std::string& captchaAnswer, const std::string& answer,
                          bool showNsfw) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"username", "\"" + username + "\""},
        {"password", "\"" + password + "\""},
        {"password_verify", "\"" + password + "\""},
        {"show_nsfw", showNsfw ? "true" : "false"},
    };
    if (!email.empty()) fields.push_back({"email", "\"" + email + "\""});
    if (!captchaUuid.empty()) fields.push_back({"captcha_uuid", "\"" + captchaUuid + "\""});
    if (!captchaAnswer.empty()) fields.push_back({"captcha_answer", "\"" + captchaAnswer + "\""});
    if (!answer.empty()) fields.push_back({"answer", "\"" + answer + "\""});
    return postJson("/api/v3/user/register", lemmyBuildJson(fields), "", 15000);
}

std::string lemmyGetCaptcha(const std::string& auth) {
    return getJson("/api/v3/user/get_captcha", auth);
}

// ==== Community ====

std::string lemmyListCommunities(const std::string& auth, const std::string& type_,
                                 const std::string& sort, int page, int limit,
                                 bool showNsfw) {
    std::string path = "/api/v3/community/list?" + lemmyBuildQuery({
        {"type_", type_},
        {"sort", sort},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"show_nsfw", showNsfw ? "true" : "false"},
    });
    return getJson(path, auth);
}

std::string lemmyGetCommunity(const std::string& auth, int id, const std::string& name) {
    std::vector<std::pair<std::string, std::string>> params;
    if (id > 0) params.push_back({"id", std::to_string(id)});
    if (!name.empty()) params.push_back({"name", name});
    std::string path = "/api/v3/community";
    if (!params.empty()) path += "?" + lemmyBuildQuery(params);
    return getJson(path, auth);
}

std::string lemmyFollowCommunity(const std::string& auth, int communityId, bool follow) {
    return postJson("/api/v3/community/follow", lemmyBuildJson({
        {"community_id", std::to_string(communityId)},
        {"follow", follow ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyBlockCommunity(const std::string& auth, int communityId, bool block) {
    return postJson("/api/v3/community/block", lemmyBuildJson({
        {"community_id", std::to_string(communityId)},
        {"block", block ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyHideCommunity(const std::string& auth, int communityId, bool hide,
                               const std::string& reason) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"community_id", std::to_string(communityId)},
        {"hidden", hide ? "true" : "false"},
    };
    if (!reason.empty()) fields.push_back({"reason", "\"" + reason + "\""});
    return postJson("/api/v3/community/hide", lemmyBuildJson(fields), auth, 15000);
}

// ==== Post ====

std::string lemmyListPosts(const std::string& auth, const std::string& type_,
                           const std::string& sort, int page, int limit,
                           int communityId, const std::string& communityName,
                           bool savedOnly, bool likedOnly, bool dislikedOnly,
                           bool showNsfw) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"type_", type_},
        {"sort", sort},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"show_nsfw", showNsfw ? "true" : "false"},
    };
    if (communityId > 0) params.push_back({"community_id", std::to_string(communityId)});
    if (!communityName.empty()) params.push_back({"community_name", communityName});
    if (savedOnly) params.push_back({"saved_only", "true"});
    if (likedOnly) params.push_back({"liked_only", "true"});
    if (dislikedOnly) params.push_back({"disliked_only", "true"});
    return getJson("/api/v3/post/list?" + lemmyBuildQuery(params), auth);
}

std::string lemmyGetPost(const std::string& auth, int postId) {
    return getJson("/api/v3/post?id=" + std::to_string(postId), auth);
}

std::string lemmyCreatePost(const std::string& auth, int communityId, const std::string& name,
                            const std::string& body, const std::string& url,
                            bool nsfw, int languageId) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"community_id", std::to_string(communityId)},
        {"name", "\"" + name + "\""},
        {"nsfw", nsfw ? "true" : "false"},
    };
    if (!body.empty()) fields.push_back({"body", "\"" + body + "\""});
    if (!url.empty()) fields.push_back({"url", "\"" + url + "\""});
    if (languageId > 0) fields.push_back({"language_id", std::to_string(languageId)});
    return postJson("/api/v3/post", lemmyBuildJson(fields), auth, 30000);
}

std::string lemmyEditPost(const std::string& auth, int postId, const std::string& name,
                          const std::string& body, const std::string& url,
                          bool nsfw, int languageId) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"post_id", std::to_string(postId)},
        {"nsfw", nsfw ? "true" : "false"},
    };
    if (!name.empty()) fields.push_back({"name", "\"" + name + "\""});
    if (!body.empty()) fields.push_back({"body", "\"" + body + "\""});
    if (!url.empty()) fields.push_back({"url", "\"" + url + "\""});
    if (languageId > 0) fields.push_back({"language_id", std::to_string(languageId)});
    return putJson("/api/v3/post", lemmyBuildJson(fields), auth, 30000);
}

std::string lemmyDeletePost(const std::string& auth, int postId, bool deleted) {
    return postJson("/api/v3/post/delete", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"deleted", deleted ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyRemovePost(const std::string& auth, int postId, bool removed,
                            const std::string& reason) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"post_id", std::to_string(postId)},
        {"removed", removed ? "true" : "false"},
    };
    if (!reason.empty()) fields.push_back({"reason", "\"" + reason + "\""});
    return postJson("/api/v3/post/remove", lemmyBuildJson(fields), auth, 15000);
}

std::string lemmyLikePost(const std::string& auth, int postId, int score) {
    return postJson("/api/v3/post/like", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"score", std::to_string(score)},
    }), auth, 15000);
}

std::string lemmySavePost(const std::string& auth, int postId, bool save) {
    return putJson("/api/v3/post/save", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"save", save ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyReportPost(const std::string& auth, int postId, const std::string& reason) {
    return postJson("/api/v3/post/report", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"reason", "\"" + reason + "\""},
    }), auth, 15000);
}

std::string lemmyLockPost(const std::string& auth, int postId, bool locked) {
    return postJson("/api/v3/post/lock", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"locked", locked ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyFeaturePost(const std::string& auth, int postId, bool featured,
                             const std::string& featureType) {
    return postJson("/api/v3/post/feature", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"featured", featured ? "true" : "false"},
        {"feature_type", "\"" + featureType + "\""},
    }), auth, 15000);
}

std::string lemmyMarkPostRead(const std::string& auth, int postId, bool read) {
    return postJson("/api/v3/post/mark_as_read", lemmyBuildJson({
        {"post_id", std::to_string(postId)},
        {"read", read ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyGetSiteMetadata(const std::string& auth, const std::string& url) {
    return getJson("/api/v3/post/site_metadata?url=" + urlEncode(url), auth);
}

// ==== Comment ====

std::string lemmyListComments(const std::string& auth, int postId, const std::string& sort,
                              int maxDepth, int limit, int page,
                              bool savedOnly, bool likedOnly, bool dislikedOnly) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"post_id", std::to_string(postId)},
        {"sort", sort},
        {"max_depth", std::to_string(maxDepth)},
        {"limit", std::to_string(limit)},
        {"page", std::to_string(page)},
    };
    if (savedOnly) params.push_back({"saved_only", "true"});
    if (likedOnly) params.push_back({"liked_only", "true"});
    if (dislikedOnly) params.push_back({"disliked_only", "true"});
    return getJson("/api/v3/comment/list?" + lemmyBuildQuery(params), auth);
}

std::string lemmyGetComment(const std::string& auth, int commentId) {
    return getJson("/api/v3/comment?id=" + std::to_string(commentId), auth);
}

std::string lemmyCreateComment(const std::string& auth, int postId, const std::string& content,
                               int parentId, int languageId) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"post_id", std::to_string(postId)},
        {"content", "\"" + content + "\""},
    };
    if (parentId > 0) fields.push_back({"parent_id", std::to_string(parentId)});
    if (languageId > 0) fields.push_back({"language_id", std::to_string(languageId)});
    return postJson("/api/v3/comment", lemmyBuildJson(fields), auth, 15000);
}

std::string lemmyEditComment(const std::string& auth, int commentId, const std::string& content,
                             int languageId) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"comment_id", std::to_string(commentId)},
        {"content", "\"" + content + "\""},
    };
    if (languageId > 0) fields.push_back({"language_id", std::to_string(languageId)});
    return putJson("/api/v3/comment", lemmyBuildJson(fields), auth, 15000);
}

std::string lemmyDeleteComment(const std::string& auth, int commentId, bool deleted) {
    return postJson("/api/v3/comment/delete", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"deleted", deleted ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyRemoveComment(const std::string& auth, int commentId, bool removed,
                               const std::string& reason) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"comment_id", std::to_string(commentId)},
        {"removed", removed ? "true" : "false"},
    };
    if (!reason.empty()) fields.push_back({"reason", "\"" + reason + "\""});
    return postJson("/api/v3/comment/remove", lemmyBuildJson(fields), auth, 15000);
}

std::string lemmyLikeComment(const std::string& auth, int commentId, int score) {
    return postJson("/api/v3/comment/like", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"score", std::to_string(score)},
    }), auth, 15000);
}

std::string lemmySaveComment(const std::string& auth, int commentId, bool save) {
    return putJson("/api/v3/comment/save", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"save", save ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyReportComment(const std::string& auth, int commentId, const std::string& reason) {
    return postJson("/api/v3/comment/report", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"reason", "\"" + reason + "\""},
    }), auth, 15000);
}

std::string lemmyDistinguishComment(const std::string& auth, int commentId, bool distinguished) {
    return postJson("/api/v3/comment/distinguish", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"distinguished", distinguished ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyMarkCommentRead(const std::string& auth, int commentId, bool read) {
    return postJson("/api/v3/comment/mark_as_read", lemmyBuildJson({
        {"comment_id", std::to_string(commentId)},
        {"read", read ? "true" : "false"},
    }), auth, 15000);
}

// ==== User ====

std::string lemmyGetUser(const std::string& auth, const std::string& username,
                         int personId, const std::string& sort,
                         int page, int limit, int communityId, bool savedOnly) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"sort", sort},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"saved_only", savedOnly ? "true" : "false"},
    };
    if (!username.empty()) params.push_back({"username", username});
    if (personId > 0) params.push_back({"person_id", std::to_string(personId)});
    if (communityId > 0) params.push_back({"community_id", std::to_string(communityId)});
    return getJson("/api/v3/user?" + lemmyBuildQuery(params), auth);
}

std::string lemmyBlockUser(const std::string& auth, int personId, bool block) {
    return postJson("/api/v3/user/block", lemmyBuildJson({
        {"person_id", std::to_string(personId)},
        {"block", block ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyGetUserMentions(const std::string& auth, const std::string& sort,
                                 int page, int limit, bool unreadOnly) {
    std::string path = "/api/v3/user/mention?" + lemmyBuildQuery({
        {"sort", sort},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"unread_only", unreadOnly ? "true" : "false"},
    });
    return getJson(path, auth);
}

std::string lemmyMarkMentionRead(const std::string& auth, int mentionId, bool read) {
    return postJson("/api/v3/user/mention/mark_as_read", lemmyBuildJson({
        {"person_mention_id", std::to_string(mentionId)},
        {"read", read ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyGetUserReplies(const std::string& auth, const std::string& sort,
                                int page, int limit, bool unreadOnly) {
    std::string path = "/api/v3/user/replies?" + lemmyBuildQuery({
        {"sort", sort},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"unread_only", unreadOnly ? "true" : "false"},
    });
    return getJson(path, auth);
}

std::string lemmyMarkAllAsRead(const std::string& auth) {
    return postJson("/api/v3/user/mark_all_as_read", "{}", auth, 15000);
}

std::string lemmyGetUnreadCount(const std::string& auth) {
    return getJson("/api/v3/user/unread_count", auth);
}

std::string lemmySaveUserSettings(const std::string& auth, const std::string& settingsJson) {
    return putJson("/api/v3/user/save_user_settings", settingsJson, auth, 15000);
}

std::string lemmyDeleteAccount(const std::string& auth, const std::string& password) {
    std::string body = password.empty() ? "{}" :
        lemmyBuildJson({{"password", "\"" + password + "\""}});
    return postJson("/api/v3/user/delete_account", body, auth, 15000);
}

std::string lemmyDeleteAccount(const std::string& auth, const std::string& password,
                                bool deleteContent) {
    return postJson("/api/v3/user/delete_account", lemmyBuildJson({
        {"password", "\"" + password + "\""},
        {"delete_content", deleteContent ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyChangePassword(const std::string& auth, const std::string& newPassword,
                                const std::string& newPasswordVerify, const std::string& oldPassword) {
    return putJson("/api/v3/user/change_password", lemmyBuildJson({
        {"new_password", "\"" + newPassword + "\""},
        {"new_password_verify", "\"" + newPasswordVerify + "\""},
        {"old_password", "\"" + oldPassword + "\""},
    }), auth, 15000);
}

// ==== Private Messages ====

std::string lemmyListPrivateMessages(const std::string& auth, int page, int limit,
                                     bool unreadOnly) {
    std::string path = "/api/v3/private_message/list?" + lemmyBuildQuery({
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"unread_only", unreadOnly ? "true" : "false"},
    });
    return getJson(path, auth);
}

std::string lemmySendPrivateMessage(const std::string& auth, int recipientId,
                                    const std::string& content) {
    return postJson("/api/v3/private_message", lemmyBuildJson({
        {"recipient_id", std::to_string(recipientId)},
        {"content", "\"" + content + "\""},
    }), auth, 15000);
}

std::string lemmyEditPrivateMessage(const std::string& auth, int messageId,
                                    const std::string& content) {
    return putJson("/api/v3/private_message", lemmyBuildJson({
        {"private_message_id", std::to_string(messageId)},
        {"content", "\"" + content + "\""},
    }), auth, 15000);
}

std::string lemmyDeletePrivateMessage(const std::string& auth, int messageId, bool deleted) {
    return postJson("/api/v3/private_message/delete", lemmyBuildJson({
        {"private_message_id", std::to_string(messageId)},
        {"deleted", deleted ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyMarkPrivateMessageRead(const std::string& auth, int messageId, bool read) {
    return postJson("/api/v3/private_message/mark_as_read", lemmyBuildJson({
        {"private_message_id", std::to_string(messageId)},
        {"read", read ? "true" : "false"},
    }), auth, 15000);
}

// ==== Search ====

std::string lemmySearch(const std::string& auth, const std::string& q,
                        const std::string& type_, const std::string& sort,
                        const std::string& listingType, int page, int limit,
                        int communityId, const std::string& communityName,
                        int creatorId) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"q", q},
        {"type_", type_},
        {"sort", sort},
        {"listing_type", listingType},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
    };
    if (communityId > 0) params.push_back({"community_id", std::to_string(communityId)});
    if (!communityName.empty()) params.push_back({"community_name", communityName});
    if (creatorId > 0) params.push_back({"creator_id", std::to_string(creatorId)});
    return getJson("/api/v3/search?" + lemmyBuildQuery(params), auth);
}

std::string lemmyResolveObject(const std::string& auth, const std::string& q) {
    return getJson("/api/v3/resolve_object?q=" + urlEncode(q), auth);
}

// ==== Moderation ====

std::string lemmyGetModlog(const std::string& auth, int modPersonId, int communityId,
                           int page, int limit, const std::string& type_,
                           int otherPersonId) {
    std::vector<std::pair<std::string, std::string>> params = {
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
        {"type_", type_},
    };
    if (modPersonId > 0) params.push_back({"mod_person_id", std::to_string(modPersonId)});
    if (communityId > 0) params.push_back({"community_id", std::to_string(communityId)});
    if (otherPersonId > 0) params.push_back({"other_person_id", std::to_string(otherPersonId)});
    return getJson("/api/v3/modlog?" + lemmyBuildQuery(params), auth);
}

// ==== Admin ====

std::string lemmyAddAdmin(const std::string& auth, int personId, bool added) {
    return postJson("/api/v3/admin/add", lemmyBuildJson({
        {"person_id", std::to_string(personId)},
        {"added", added ? "true" : "false"},
    }), auth, 15000);
}

std::string lemmyListRegistrationApplications(const std::string& auth, bool unreadOnly,
                                              int page, int limit) {
    std::string path = "/api/v3/admin/registration_application/list?" + lemmyBuildQuery({
        {"unread_only", unreadOnly ? "true" : "false"},
        {"page", std::to_string(page)},
        {"limit", std::to_string(limit)},
    });
    return getJson(path, auth);
}

std::string lemmyApproveRegistration(const std::string& auth, int id, bool approve,
                                     const std::string& denyReason) {
    std::vector<std::pair<std::string, std::string>> fields = {
        {"id", std::to_string(id)},
        {"approve", approve ? "true" : "false"},
    };
    if (!denyReason.empty()) fields.push_back({"deny_reason", "\"" + denyReason + "\""});
    return postJson("/api/v3/admin/registration_application/approve",
                    lemmyBuildJson(fields), auth, 15000);
}

std::string lemmyCreateSite(const std::string& auth, const std::string& siteJson) {
    return postJson("/api/v3/site", siteJson, auth, 15000);
}

std::string lemmyBlockInstance(const std::string& auth, int instanceId, bool block) {
    return postJson("/api/v3/site/block", lemmyBuildJson({
        {"instance_id", std::to_string(instanceId)},
        {"block", block ? "true" : "false"},
    }), auth, 15000);
}

// ==== Image Upload ====

std::string lemmyUploadImage(const std::string& auth, const std::string& imageData,
                             const std::string& filename, const std::string& mimeType) {
    // Build multipart/form-data body for Pictrs
    // field name: "images[]" (array of images)
    static const std::string boundary = "----LemmyNativeUploadBoundary";
    static const std::string crlf = "\r\n";

    // Determine MIME type from extension if not provided
    std::string contentType = mimeType;
    if (contentType.empty()) {
        auto dot = filename.rfind('.');
        if (dot != std::string::npos) {
            std::string ext = filename.substr(dot);
            if (ext == ".jpg" || ext == ".jpeg") contentType = "image/jpeg";
            else if (ext == ".png") contentType = "image/png";
            else if (ext == ".gif") contentType = "image/gif";
            else if (ext == ".webp") contentType = "image/webp";
            else if (ext == ".svg") contentType = "image/svg+xml";
            else contentType = "application/octet-stream";
        } else {
            contentType = "application/octet-stream";
        }
    }

    std::string body;
    body += "--" + boundary + crlf;
    body += "Content-Disposition: form-data; name=\"images[]\"; filename=\"" + filename + "\"" + crlf;
    body += "Content-Type: " + contentType + crlf;
    body += crlf;
    body += imageData;
    body += crlf;
    body += "--" + boundary + "--" + crlf;

    std::unordered_map<std::string, std::string> headers;
    headers["Content-Type"] = "multipart/form-data; boundary=" + boundary;
    if (!auth.empty()) headers["Authorization"] = "Bearer " + auth;

    HttpRequest req{"POST", lemmyBuildUrl("/pictrs/image"), body, headers, 60000};
    return httpExecute(req).body;
}

} // namespace progressive
