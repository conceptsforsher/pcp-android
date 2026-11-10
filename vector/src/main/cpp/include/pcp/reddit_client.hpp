#pragma once

#include "reddit_types.hpp"
#include <string>
#include <functional>

namespace pcp {
namespace reddit {

class RedditClient {
public:
    RedditClient() = default;

    // Data parsing (no networking - HTTP is handled on Kotlin side)
    static std::string parsePosts(const std::string& json, std::string& outAfter);
    static std::string parseComments(const std::string& json);
    static std::string parseSubreddits(const std::string& json);
    static std::string parseSubredditInfo(const std::string& json);
    static std::string parseUserAbout(const std::string& json);

    // URL builders
    static std::string frontpageUrl(SortOrder sort, const std::string& after, int limit = 25);
    static std::string subredditUrl(const std::string& sub, SortOrder sort, const std::string& after);
    static std::string commentsUrl(const std::string& sub, const std::string& postId);
    static std::string userPostsUrl(const std::string& user, SortOrder sort);
    static std::string subredditSearchUrl(const std::string& query);
    static std::string subredditAboutUrl(const std::string& sub);
    static std::string userAboutUrl(const std::string& user);
    static std::string voteUrl();
    static std::string saveUrl(bool saveFlag);
    static std::string hideUrl();
    static std::string submitCommentUrl();
    static std::string submitPostUrl();
    static std::string tokenUrl();

    // OAuth helpers
    static std::string buildOAuthBody(const std::string& action, const std::string& id,
                                      const std::string& extraKey = "", const std::string& extraVal = "");
    static std::string buildSubmitBody(const std::string& kind, const std::string& sr,
                                       const std::string& title, const std::string& url,
                                       const std::string& text);
};

} // namespace reddit
} // namespace pcp
