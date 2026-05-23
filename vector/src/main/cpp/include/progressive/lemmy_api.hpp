#pragma once

#include <string>
#include <vector>
#include "progressive/lemmy_types.hpp"

namespace progressive {

// ==== Lemmy Native API Client ====
//
// C++ client for the Lemmy HTTP API (v3).
// Uses progressive::http_client for transport.
// All functions return raw JSON response body as std::string.
// Auth token is passed per-call (stateless design).
//
// API docs: https://join-lemmy.org/docs/contributors/04-api.html
//           https://lemmy.readme.io/

// ==== Instance & Config ====

void setLemmyInstance(const std::string& instanceUrl);
const std::string& getLemmyInstance();

// Build full Lemmy API URL from endpoint path
// e.g. lemmyBuildUrl("/site") → "https://lemmy.ml/api/v3/site"
std::string lemmyBuildUrl(const std::string& path);

// ==== Site ====

// GET /api/v3/site — Get site info and your user data (if authed)
// auth: JWT token or empty for anonymous
std::string lemmyGetSite(const std::string& auth = "");

// ==== Auth ====

// POST /api/v3/user/login — Login with username/password
// Returns JSON: {"jwt":"...","registration_created":false,"verify_email_sent":false}
std::string lemmyLogin(const std::string& username, const std::string& password,
                       const std::string& totp2faToken = "");

// POST /api/v3/user/register — Register a new user
// Returns JSON: {"jwt":"..."} on success (if verification not required)
std::string lemmyRegister(const std::string& username, const std::string& password,
                          const std::string& email = "",
                          const std::string& captchaUuid = "",
                          const std::string& captchaAnswer = "",
                          const std::string& answer = "",
                          bool showNsfw = false);

// GET /api/v3/user/get_captcha — Get captcha info
std::string lemmyGetCaptcha(const std::string& auth = "");

// ==== Community ====

// GET /api/v3/community/list — List communities
// type_: "Local", "All", "Subscribed"
// sort: "Active", "Hot", "New", "Old", "TopDay", "TopWeek", "TopMonth", "TopYear", "TopAll", "MostComments", "NewComments"
std::string lemmyListCommunities(const std::string& auth, const std::string& type_ = "Local",
                                 const std::string& sort = "Active", int page = 1, int limit = 20,
                                 bool showNsfw = false);

// GET /api/v3/community — Get community by id or name
// Provide either id or name (name includes !community@instance format)
std::string lemmyGetCommunity(const std::string& auth, int id = 0, const std::string& name = "");

// POST /api/v3/community/follow — Follow / unfollow a community
std::string lemmyFollowCommunity(const std::string& auth, int communityId, bool follow);

// POST /api/v3/community/block — Block / unblock a community
std::string lemmyBlockCommunity(const std::string& auth, int communityId, bool block);

// POST /api/v3/community/hide — Hide community from all view (mods/admins)
std::string lemmyHideCommunity(const std::string& auth, int communityId, bool hide,
                               const std::string& reason = "");

// ==== Post ====

// GET /api/v3/post/list — List posts
// type_: "All", "Local", "Subscribed", "ModeratorView"
// sort: "Active", "Hot", "New", "Old", "TopDay", "TopWeek", "TopMonth", "TopYear", "TopAll",
//       "MostComments", "NewComments", "TopHour", "TopSixHour", "TopTwelveHour",
//       "TopThreeMonths", "TopSixMonths", "TopNineMonths", "Controversial", "Scaled"
std::string lemmyListPosts(const std::string& auth, const std::string& type_ = "Local",
                           const std::string& sort = "Active", int page = 1, int limit = 20,
                           int communityId = 0, const std::string& communityName = "",
                           bool savedOnly = false, bool likedOnly = false,
                           bool dislikedOnly = false, bool showNsfw = false);

// GET /api/v3/post — Get single post by id
std::string lemmyGetPost(const std::string& auth, int postId);

// POST /api/v3/post — Create a post
// body: markdown text; url: link URL; either body or url (or both) must be set
std::string lemmyCreatePost(const std::string& auth, int communityId, const std::string& name,
                            const std::string& body = "", const std::string& url = "",
                            bool nsfw = false, int languageId = 0);

// PUT /api/v3/post — Edit a post
std::string lemmyEditPost(const std::string& auth, int postId, const std::string& name = "",
                          const std::string& body = "", const std::string& url = "",
                          bool nsfw = false, int languageId = 0);

// POST /api/v3/post/delete — Delete / restore a post
std::string lemmyDeletePost(const std::string& auth, int postId, bool deleted);

// POST /api/v3/post/remove — Mod remove / restore a post
std::string lemmyRemovePost(const std::string& auth, int postId, bool removed,
                            const std::string& reason = "");

// POST /api/v3/post/like — Vote on a post: 1 = up, 0 = neutral, -1 = down
std::string lemmyLikePost(const std::string& auth, int postId, int score);

// PUT /api/v3/post/save — Save / unsave a post
std::string lemmySavePost(const std::string& auth, int postId, bool save);

// POST /api/v3/post/report — Report a post
std::string lemmyReportPost(const std::string& auth, int postId, const std::string& reason);

// POST /api/v3/post/lock — Lock / unlock a post (mods)
std::string lemmyLockPost(const std::string& auth, int postId, bool locked);

// POST /api/v3/post/feature — Feature / unfeature a post (mods)
// featureType: "Local" or "Community"
std::string lemmyFeaturePost(const std::string& auth, int postId, bool featured,
                             const std::string& featureType = "Local");

// POST /api/v3/post/mark_as_read — Mark a post as read
std::string lemmyMarkPostRead(const std::string& auth, int postId, bool read);

// GET /api/v3/post/site_metadata — Fetch metadata for a URL
std::string lemmyGetSiteMetadata(const std::string& auth, const std::string& url);

// ==== Comment ====

// GET /api/v3/comment/list — List comments for a post
// sort: "Hot", "Top", "New", "Old", "Controversial"
// maxDepth: max nesting depth (0 = no limit)
std::string lemmyListComments(const std::string& auth, int postId, const std::string& sort = "Hot",
                              int maxDepth = 8, int limit = 50, int page = 1,
                              bool savedOnly = false, bool likedOnly = false,
                              bool dislikedOnly = false);

// GET /api/v3/comment — Get single comment by id
std::string lemmyGetComment(const std::string& auth, int commentId);

// POST /api/v3/comment — Create a comment on a post (or reply to another comment)
// parentId: parent comment id for a reply (0 = top-level)
std::string lemmyCreateComment(const std::string& auth, int postId, const std::string& content,
                               int parentId = 0, int languageId = 0);

// PUT /api/v3/comment — Edit a comment
std::string lemmyEditComment(const std::string& auth, int commentId, const std::string& content,
                             int languageId = 0);

// POST /api/v3/comment/delete — Delete / restore a comment
std::string lemmyDeleteComment(const std::string& auth, int commentId, bool deleted);

// POST /api/v3/comment/remove — Mod remove / restore a comment
std::string lemmyRemoveComment(const std::string& auth, int commentId, bool removed,
                               const std::string& reason = "");

// POST /api/v3/comment/like — Vote on a comment
std::string lemmyLikeComment(const std::string& auth, int commentId, int score);

// PUT /api/v3/comment/save — Save / unsave a comment
std::string lemmySaveComment(const std::string& auth, int commentId, bool save);

// POST /api/v3/comment/report — Report a comment
std::string lemmyReportComment(const std::string& auth, int commentId, const std::string& reason);

// POST /api/v3/comment/distinguish — Distinguish / undistinguish (mods)
std::string lemmyDistinguishComment(const std::string& auth, int commentId, bool distinguished);

// POST /api/v3/comment/mark_as_read — Mark a comment as read
std::string lemmyMarkCommentRead(const std::string& auth, int commentId, bool read);

// ==== User ====

// GET /api/v3/user — Get user details
// Provide either username or personId
std::string lemmyGetUser(const std::string& auth, const std::string& username = "",
                         int personId = 0, const std::string& sort = "New",
                         int page = 1, int limit = 20, int communityId = 0,
                         bool savedOnly = false);

// POST /api/v3/user/block — Block / unblock a user
std::string lemmyBlockUser(const std::string& auth, int personId, bool block);

// GET /api/v3/user/mention — Get mentions for current user
std::string lemmyGetUserMentions(const std::string& auth, const std::string& sort = "New",
                                 int page = 1, int limit = 20, bool unreadOnly = false);

// POST /api/v3/user/mention/mark_as_read — Mark a mention as read
std::string lemmyMarkMentionRead(const std::string& auth, int mentionId, bool read);

// GET /api/v3/user/replies — Get comment replies to current user
std::string lemmyGetUserReplies(const std::string& auth, const std::string& sort = "New",
                                int page = 1, int limit = 20, bool unreadOnly = false);

// POST /api/v3/user/mark_all_as_read — Mark all replies as read
std::string lemmyMarkAllAsRead(const std::string& auth);

// GET /api/v3/user/unread_count — Get unread counts
std::string lemmyGetUnreadCount(const std::string& auth);

// PUT /api/v3/user/save_user_settings — Save user settings
std::string lemmySaveUserSettings(const std::string& auth, const std::string& settingsJson);

// POST /api/v3/user/delete_account — Delete current user account
std::string lemmyDeleteAccount(const std::string& auth, const std::string& password = "");
std::string lemmyDeleteAccount(const std::string& auth, const std::string& password,
                                bool deleteContent);

// POST /api/v3/user/password_change — Change password
std::string lemmyChangePassword(const std::string& auth, const std::string& newPassword,
                                const std::string& newPasswordVerify, const std::string& oldPassword);

// ==== Private Messages ====

// GET /api/v3/private_message/list — List private messages
std::string lemmyListPrivateMessages(const std::string& auth, int page = 1, int limit = 20,
                                     bool unreadOnly = false);

// POST /api/v3/private_message — Send a private message
std::string lemmySendPrivateMessage(const std::string& auth, int recipientId,
                                    const std::string& content);

// PUT /api/v3/private_message — Edit a private message
std::string lemmyEditPrivateMessage(const std::string& auth, int messageId,
                                    const std::string& content);

// POST /api/v3/private_message/delete — Delete a private message
std::string lemmyDeletePrivateMessage(const std::string& auth, int messageId, bool deleted);

// POST /api/v3/private_message/mark_as_read — Mark PM as read
std::string lemmyMarkPrivateMessageRead(const std::string& auth, int messageId, bool read);

// ==== Search ====

// GET /api/v3/search — Search Lemmy
// q: search query
// type_: "All", "Comments", "Posts", "Communities", "Users", "Url"
// sort: "TopAll", "TopYear", "TopMonth", "TopWeek", "TopDay", "New", "Old", "Controversial", "Hot", "Active"
// listingType: "All", "Local", "Subscribed", "ModeratorView"
std::string lemmySearch(const std::string& auth, const std::string& q,
                        const std::string& type_ = "All", const std::string& sort = "TopAll",
                        const std::string& listingType = "All", int page = 1, int limit = 20,
                        int communityId = 0, const std::string& communityName = "",
                        int creatorId = 0);

// GET /api/v3/resolve_object — Resolve a federated object by URL
// Example: resolve "https://lemmy.ml/post/123" → returns full post/comment/community/user data
std::string lemmyResolveObject(const std::string& auth, const std::string& q);

// ==== Moderation ====

// GET /api/v3/modlog — Get moderation log
std::string lemmyGetModlog(const std::string& auth, int modPersonId = 0, int communityId = 0,
                           int page = 1, int limit = 20, const std::string& type_ = "All",
                           int otherPersonId = 0);

// ==== Admin ====

// POST /api/v3/admin/add — Add an admin
std::string lemmyAddAdmin(const std::string& auth, int personId, bool added);

// GET /api/v3/admin/registration_application/list — List registration applications
std::string lemmyListRegistrationApplications(const std::string& auth, bool unreadOnly = false,
                                              int page = 1, int limit = 20);

// PUT /api/v3/admin/registration_application/approve — Approve / deny registration
std::string lemmyApproveRegistration(const std::string& auth, int id, bool approve,
                                     const std::string& denyReason = "");

// POST /api/v3/site — Create / edit site config
std::string lemmyCreateSite(const std::string& auth, const std::string& siteJson);

// POST /api/v3/site/block — Block / unblock an instance
std::string lemmyBlockInstance(const std::string& auth, int instanceId, bool block);

// ==== Image Upload ====

// POST /pictrs/image — Upload an image file (multipart/form-data)
// imageData: raw file bytes; filename: e.g. "photo.jpg"
// mimeType: e.g. "image/jpeg" or empty for auto-detect
// Returns JSON: {"files":[{"delete_token":"...","file":"..."}],"msg":"ok"}
std::string lemmyUploadImage(const std::string& auth, const std::string& imageData,
                             const std::string& filename, const std::string& mimeType = "");

// ==== Utility ====

// Build a simple JSON object from key-value string pairs.
// Values must already be JSON-encoded (quoted strings, numbers, booleans).
// Example: buildJson({{"community_id", "5"}, {"name", "\"Hello\""}})
//   → {"community_id":5,"name":"Hello"}
std::string lemmyBuildJson(const std::vector<std::pair<std::string, std::string>>& fields);

// Build query string from parameters
std::string lemmyBuildQuery(const std::vector<std::pair<std::string, std::string>>& params);

} // namespace progressive
