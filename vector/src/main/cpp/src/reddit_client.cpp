#include "pcp/reddit_client.hpp"
#include <sstream>
#include <cstring>

namespace pcp {
namespace reddit {

// Simple JSON string builder (no dependency on heavy JSON library)
static inline std::string jsonStr(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else if (c == '\n') out += "\\n";
        else out += c;
    }
    out += "\"";
    return out;
}

static inline std::string jsonInt(int64_t n) { return std::to_string(n); }
static inline std::string jsonBool(bool b) { return b ? "true" : "false"; }

// Basic JSON field extractor (no full parser needed for our use case)
static std::string extractField(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";

    pos += search.length();
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t')) pos++;

    if (pos >= json.length()) return "";

    if (json[pos] == '"') {
        size_t start = pos + 1;
        size_t end = json.find('"', start);
        if (end == std::string::npos) return "";
        std::string val = json.substr(start, end - start);
        // Unescape
        std::string out;
        for (size_t i = 0; i < val.length(); i++) {
            if (val[i] == '\\' && i + 1 < val.length()) {
                i++;
                if (val[i] == 'n') out += '\n';
                else if (val[i] == '"') out += '"';
                else out += val[i];
            } else {
                out += val[i];
            }
        }
        return out;
    } else if (json[pos] == 't' || json[pos] == 'f' || json[pos] == 'n') {
        if (json.substr(pos, 4) == "true") return "true";
        if (json.substr(pos, 5) == "false") return "false";
        if (json.substr(pos, 4) == "null") return "";
        return json.substr(pos, 1);
    } else {
        size_t end = pos;
        while (end < json.length() && json[end] != ',' && json[end] != '}' && json[end] != ']') end++;
        return json.substr(pos, end - pos);
    }
}

static int extractInt(const std::string& json, const std::string& key) {
    std::string v = extractField(json, key);
    if (v.empty()) return 0;
    try { return std::stoi(v); } catch (...) { return 0; }
}

static int64_t extractInt64(const std::string& json, const std::string& key) {
    std::string v = extractField(json, key);
    if (v.empty()) return 0;
    try { return std::stoll(v); } catch (...) { return 0; }
}

static bool extractBool(const std::string& json, const std::string& key) {
    return extractField(json, key) == "true";
}

static std::string sortToString(SortOrder sort) {
    switch (sort) {
    case SortOrder::New: return "new";
    case SortOrder::Top: return "top";
    case SortOrder::Rising: return "rising";
    case SortOrder::Controversial: return "controversial";
    default: return "hot";
    }
}

// ==== URL builders ====

static const char* ANON_BASE = "https://old.reddit.com";
static const char* OAUTH_BASE = "https://oauth.reddit.com";

std::string RedditClient::frontpageUrl(SortOrder sort, const std::string& after, int limit) {
    std::ostringstream oss;
    oss << ANON_BASE << "/" << sortToString(sort) << ".json?limit=" << limit << "&raw_json=1";
    if (!after.empty()) oss << "&after=" << after;
    return oss.str();
}

std::string RedditClient::subredditUrl(const std::string& sub, SortOrder sort, const std::string& after) {
    std::ostringstream oss;
    oss << ANON_BASE << "/r/" << sub << "/" << sortToString(sort) << ".json?limit=25&raw_json=1";
    if (!after.empty()) oss << "&after=" << after;
    return oss.str();
}

std::string RedditClient::commentsUrl(const std::string& sub, const std::string& postId) {
    return std::string(ANON_BASE) + "/r/" + sub + "/comments/" + postId + ".json?limit=200&raw_json=1&sort=confidence";
}

std::string RedditClient::userPostsUrl(const std::string& user, SortOrder sort) {
    return std::string(ANON_BASE) + "/user/" + user + "/submitted.json?limit=25&sort=" + sortToString(sort);
}

std::string RedditClient::subredditSearchUrl(const std::string& query) {
    return std::string(ANON_BASE) + "/subreddits/search.json?q=" + query + "&limit=50";
}

std::string RedditClient::subredditAboutUrl(const std::string& sub) {
    return std::string(ANON_BASE) + "/r/" + sub + "/about.json";
}

std::string RedditClient::userAboutUrl(const std::string& user) {
    return std::string(ANON_BASE) + "/user/" + user + "/about.json";
}

std::string RedditClient::voteUrl() { return std::string(OAUTH_BASE) + "/api/vote"; }
std::string RedditClient::saveUrl(bool saveFlag) {
    return std::string(OAUTH_BASE) + (saveFlag ? "/api/save" : "/api/unsave");
}
std::string RedditClient::hideUrl() { return std::string(OAUTH_BASE) + "/api/hide"; }
std::string RedditClient::submitCommentUrl() { return std::string(OAUTH_BASE) + "/api/comment"; }
std::string RedditClient::submitPostUrl() { return std::string(OAUTH_BASE) + "/api/submit"; }
std::string RedditClient::tokenUrl() { return "https://www.reddit.com/api/v1/access_token"; }

std::string RedditClient::buildOAuthBody(const std::string& action, const std::string& id,
                                          const std::string& extraKey, const std::string& extraVal) {
    std::ostringstream oss;
    if (action == "vote")
        oss << "id=" << id << "&dir=" << extraVal << "&rank=2";
    else if (action == "save")
        oss << "id=" << id;
    else if (action == "unsave")
        oss << "id=" << id;
    else if (action == "hide")
        oss << "id=" << id;
    else if (action == "comment")
        oss << "thing_id=" << id << "&text=" << extraVal << "&api_type=json";
    return oss.str();
}

std::string RedditClient::buildSubmitBody(const std::string& kind, const std::string& sr,
                                           const std::string& title, const std::string& url,
                                           const std::string& text) {
    std::ostringstream oss;
    oss << "api_type=json&kind=" << kind << "&sr=" << sr << "&title=" << title;
    if (kind == "link" && !url.empty()) oss << "&url=" << url;
    if (kind == "self" && !text.empty()) oss << "&text=" << text;
    return oss.str();
}

// ==== Data parsing ====

std::string RedditClient::parsePosts(const std::string& rawJson, std::string& outAfter) {
    std::string data = extractField(rawJson, "data");
    if (data.empty()) return "[]";

    outAfter = extractField(data, "after");
    std::string children = extractField(data, "children");
    if (children.empty()) return "[]";

    // Extract array of posts
    std::string result = "[";
    bool first = true;

    size_t pos = 0;
    while ((pos = children.find("\"kind\": \"t3\"", pos)) != std::string::npos) {
        // Find the start of this object
        size_t objStart = children.rfind("{", pos);
        if (objStart == std::string::npos) { pos++; continue; }

        // Find matching closing brace
        int depth = 0;
        size_t objEnd = objStart;
        for (size_t i = objStart; i < children.length(); i++) {
            if (children[i] == '{') depth++;
            else if (children[i] == '}') { depth--; if (depth == 0) { objEnd = i + 1; break; } }
        }

        std::string childObj = children.substr(objStart, objEnd - objStart);
        std::string childData = extractField(childObj, "data");

        if (!first) result += ",";
        first = false;

        result += "{";
        result += "\"id\":" + jsonStr(extractField(childData, "id")) + ",";
        result += "\"fullname\":" + jsonStr(extractField(childData, "name")) + ",";
        result += "\"title\":" + jsonStr(extractField(childData, "title")) + ",";
        result += "\"author\":" + jsonStr(extractField(childData, "author")) + ",";
        result += "\"subreddit\":" + jsonStr(extractField(childData, "subreddit")) + ",";
        result += "\"url\":" + jsonStr(extractField(childData, "url")) + ",";
        result += "\"thumbnail\":" + jsonStr(extractField(childData, "thumbnail")) + ",";
        result += "\"domain\":" + jsonStr(extractField(childData, "domain")) + ",";
        result += "\"permalink\":" + jsonStr(extractField(childData, "permalink")) + ",";
        result += "\"selftext\":" + jsonStr(extractField(childData, "selftext")) + ",";
        result += "\"score\":" + jsonInt(extractInt(childData, "score")) + ",";
        result += "\"commentCount\":" + jsonInt(extractInt(childData, "num_comments")) + ",";
        result += "\"nsfw\":" + jsonBool(extractBool(childData, "over_18")) + ",";
        result += "\"spoiler\":" + jsonBool(extractBool(childData, "spoiler")) + ",";
        result += "\"stickied\":" + jsonBool(extractBool(childData, "stickied")) + ",";
        result += "\"isSelf\":" + jsonBool(extractBool(childData, "is_self")) + ",";
        result += "\"isGallery\":" + jsonBool(extractBool(childData, "is_gallery")) + ",";
        result += "\"isVideo\":" + jsonBool(extractBool(childData, "is_video")) + ",";
        result += "\"createdUtc\":" + jsonInt(extractInt64(childData, "created_utc"));
        result += "}";

        pos = objEnd;
    }

    result += "]";
    return result;
}

std::string RedditClient::parseComments(const std::string& rawJson) {
    // rawJson is an array: [postListing, commentListing]
    // Extract the second element's data.children
    std::string result = "[";

    // Find the second listing
    size_t firstBracket = rawJson.find('[');
    if (firstBracket == std::string::npos) return "[]";

    // Find the second object after a comma
    size_t comma = rawJson.find(',', firstBracket);
    if (comma == std::string::npos) return "[]";

    // Parse from the second object - simple approach: look for "kind": "t1"
    size_t pos = comma;
    bool first = true;
    while ((pos = rawJson.find("\"kind\": \"t1\"", pos)) != std::string::npos) {
        size_t objStart = rawJson.rfind("{", pos);
        if (objStart == std::string::npos) { pos++; continue; }

        int depth = 0;
        size_t objEnd = objStart;
        for (size_t i = objStart; i < rawJson.length(); i++) {
            if (rawJson[i] == '{') depth++;
            else if (rawJson[i] == '}') { depth--; if (depth == 0) { objEnd = i + 1; break; } }
        }

        std::string childData = extractField(rawJson.substr(objStart, objEnd - objStart), "data");

        if (!first) result += ",";
        first = false;

        result += "{";
        result += "\"id\":" + jsonStr(extractField(childData, "id")) + ",";
        result += "\"fullname\":" + jsonStr(extractField(childData, "name")) + ",";
        result += "\"author\":" + jsonStr(extractField(childData, "author")) + ",";
        result += "\"body\":" + jsonStr(extractField(childData, "body")) + ",";
        result += "\"score\":" + jsonInt(extractInt(childData, "score")) + ",";
        result += "\"depth\":" + jsonInt(extractInt(childData, "depth")) + ",";
        result += "\"stickied\":" + jsonBool(extractBool(childData, "stickied")) + ",";
        result += "\"isSubmitter\":" + jsonBool(extractBool(childData, "is_submitter")) + ",";
        result += "\"createdUtc\":" + jsonInt(extractInt64(childData, "created_utc"));
        result += "}";

        pos = objEnd;
    }

    result += "]";
    return result;
}

std::string RedditClient::parseSubreddits(const std::string& rawJson) {
    std::string data = extractField(rawJson, "data");
    if (data.empty()) return "[]";

    std::string children = extractField(data, "children");
    if (children.empty()) return "[]";

    std::string result = "[";
    bool first = true;

    size_t pos = 0;
    while ((pos = children.find("\"kind\": \"t5\"", pos)) != std::string::npos) {
        size_t objStart = children.rfind("{", pos);
        if (objStart == std::string::npos) { pos++; continue; }

        int depth = 0;
        size_t objEnd = objStart;
        for (size_t i = objStart; i < children.length(); i++) {
            if (children[i] == '{') depth++;
            else if (children[i] == '}') { depth--; if (depth == 0) { objEnd = i + 1; break; } }
        }

        std::string childData = extractField(children.substr(objStart, objEnd - objStart), "data");

        if (!first) result += ",";
        first = false;

        result += "{";
        result += "\"name\":" + jsonStr(extractField(childData, "display_name")) + ",";
        result += "\"title\":" + jsonStr(extractField(childData, "title")) + ",";
        result += "\"description\":" + jsonStr(extractField(childData, "public_description")) + ",";
        result += "\"subscriberCount\":" + jsonInt(extractInt(childData, "subscribers")) + ",";
        result += "\"activeUserCount\":" + jsonInt(extractInt(childData, "accounts_active")) + ",";
        result += "\"over18\":" + jsonBool(extractBool(childData, "over18"));
        result += "}";

        pos = objEnd;
    }

    result += "]";
    return result;
}

std::string RedditClient::parseSubredditInfo(const std::string& rawJson) {
    std::string data = extractField(rawJson, "data");
    if (data.empty()) return "{}";

    std::string result = "{";
    result += "\"name\":" + jsonStr(extractField(data, "display_name")) + ",";
    result += "\"title\":" + jsonStr(extractField(data, "title")) + ",";
    result += "\"description\":" + jsonStr(extractField(data, "public_description")) + ",";
    result += "\"subscriberCount\":" + jsonInt(extractInt(data, "subscribers")) + ",";
    result += "\"activeUserCount\":" + jsonInt(extractInt(data, "accounts_active")) + ",";
    result += "\"over18\":" + jsonBool(extractBool(data, "over18"));
    result += "}";
    return result;
}

std::string RedditClient::parseUserAbout(const std::string& rawJson) {
    std::string data = extractField(rawJson, "data");
    if (data.empty()) return "{}";

    std::string result = "{";
    result += "\"name\":" + jsonStr(extractField(data, "name")) + ",";
    result += "\"linkKarma\":" + jsonInt(extractInt(data, "link_karma")) + ",";
    result += "\"commentKarma\":" + jsonInt(extractInt(data, "comment_karma")) + ",";
    result += "\"createdUtc\":" + jsonInt(extractInt64(data, "created_utc"));
    result += "}";
    return result;
}

} // namespace reddit
} // namespace pcp
