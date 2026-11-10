#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>

namespace pcp {
namespace reddit {

enum class PostType { Self, Link, Image, Video, Gallery, Unknown };
enum class SortOrder { Hot, New, Top, Rising, Controversial };
enum class VoteState { None, Upvoted, Downvoted };
enum class ConnectionState { Disconnected, Connecting, Connected, Error };

struct RedditPost {
    std::string id;
    std::string fullname;
    std::string title;
    std::string selftext;
    std::string author;
    std::string subreddit;
    std::string permalink;
    std::string url;
    std::string thumbnail;
    std::string domain;
    int score = 0;
    int commentCount = 0;
    bool nsfw = false;
    bool spoiler = false;
    bool stickied = false;
    bool isSelf = false;
    bool isGallery = false;
    bool isVideo = false;
    int64_t createdUtc = 0;
    VoteState voteState = VoteState::None;
    bool saved = false;

    std::string toJson() const;
    static RedditPost fromJson(const std::string& json);
};

struct RedditComment {
    std::string id;
    std::string fullname;
    std::string parentId;
    std::string author;
    std::string body;
    int score = 0;
    int depth = 0;
    int64_t createdUtc = 0;
    bool stickied = false;
    bool isSubmitter = false;

    std::string toJson() const;
    static RedditComment fromJson(const std::string& json);
};

struct RedditSubreddit {
    std::string name;
    std::string title;
    std::string description;
    int subscriberCount = 0;
    int activeUserCount = 0;
    bool over18 = false;

    std::string toJson() const;
    static RedditSubreddit fromJson(const std::string& json);
};

struct RedditAccount {
    std::string username;
    std::string accessToken;
    std::string refreshToken;
    bool isAnonymous = true;
};

using MessageCallback = std::function<void(const std::string& json)>;
using StateCallback = std::function<void(ConnectionState)>;
using ErrorCallback = std::function<void(int code, const std::string& message)>;
using PostListCallback = std::function<void(const std::string& json, const std::string& after)>;
using CommentListCallback = std::function<void(const std::string& json)>;

} // namespace reddit
} // namespace pcp
