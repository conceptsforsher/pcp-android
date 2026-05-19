#include "progressive/user_directory.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

namespace progressive {

// ====== JSON helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

UserDirectoryManager::UserDirectoryManager() {}

// ====== Search Request ======

std::string UserDirectoryManager::buildSearchRequest(const UserSearchQuery& query) const {
    std::ostringstream os;
    os << R"({"search_term":")" << query.searchTerm
       << R"(","limit":)" << query.limit;
    if (!query.serverFilter.empty()) {
        os << R"(,"server":")" << query.serverFilter << R"(")";
    }
    os << "}";
    return os.str();
}

// ====== Search Response Parsing ======

UserSearchResponse UserDirectoryManager::parseSearchResponse(const std::string& json) const {
    UserSearchResponse resp;

    // Check for error
    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        resp.error = err + ": " + extractStr(json, "error");
        return resp;
    }

    resp.limited = extractBool(json, "limited");

    // Parse results array
    size_t pos = json.find("\"results\"");
    if (pos == std::string::npos) return resp;

    pos = json.find('[', pos);
    if (pos == std::string::npos) return resp;

    pos++;
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
        if (pos >= json.size() || json[pos] == ']') break;

        size_t objStart = pos;
        int depth = 0;
        while (pos < json.size()) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            if (depth == 0 && json[pos] == '}') { pos++; break; }
            pos++;
        }
        std::string userJson = json.substr(objStart, pos - objStart);

        UserSearchResult user;
        user.userId = extractStr(userJson, "user_id");
        user.displayName = extractStr(userJson, "display_name");
        user.avatarUrl = extractStr(userJson, "avatar_url");
        user.matrixId = user.userId;
        user.isValid = !user.userId.empty();

        if (user.isValid) resp.results.push_back(user);
    }

    resp.totalResults = static_cast<int>(resp.results.size());
    return resp;
}

// ====== Search ======

UserSearchResponse UserDirectoryManager::search(const UserSearchQuery& query, const std::string& responseJson) {
    auto resp = parseSearchResponse(responseJson);
    if (!resp.error.empty()) return resp;

    // Filter self
    if (query.excludeSelf && !query.currentUserId.empty()) {
        resp.results.erase(
            std::remove_if(resp.results.begin(), resp.results.end(),
                [&](const UserSearchResult& r) { return r.userId == query.currentUserId; }),
            resp.results.end());
    }

    // Deduplicate
    deduplicate(resp.results);

    // Calculate relevance
    for (auto& r : resp.results) {
        r.relevanceScore = calculateRelevance(r, query.searchTerm);
    }

    // Sort
    sortByRelevance(resp.results, query.searchTerm);

    resp.totalResults = static_cast<int>(resp.results.size());

    // Cache results
    for (const auto& r : resp.results) {
        cache_[r.userId] = r;
    }

    return resp;
}

// ====== Deduplication ======

void UserDirectoryManager::deduplicate(std::vector<UserSearchResult>& results) const {
    std::unordered_map<std::string, UserSearchResult> unique;
    for (const auto& r : results) {
        auto it = unique.find(r.userId);
        if (it == unique.end()) {
            unique[r.userId] = r;
        } else if (r.relevanceScore > it->second.relevanceScore) {
            unique[r.userId] = r;
        }
    }

    results.clear();
    for (const auto& [id, user] : unique) results.push_back(user);
}

// ====== Tokenization ======

std::vector<std::string> UserDirectoryManager::tokenize(const std::string& text) const {
    std::vector<std::string> tokens;
    std::string current;
    for (char c : text) {
        if (c == ' ' || c == ',' || c == '.') {
            if (!current.empty()) tokens.push_back(current);
            current.clear();
        } else {
            current += static_cast<char>(std::tolower(c));
        }
    }
    if (!current.empty()) tokens.push_back(current);
    return tokens;
}

// ====== Relevance Calculation ======

int UserDirectoryManager::calculateRelevance(const UserSearchResult& result, const std::string& query) const {
    if (query.empty()) return 0;

    std::string q;
    for (char c : query) q += static_cast<char>(std::tolower(c));

    std::string name;
    for (char c : result.displayName) name += static_cast<char>(std::tolower(c));

    std::string uid;
    for (char c : result.userId) uid += static_cast<char>(std::tolower(c));

    int score = 0;

    // Exact match on display name (highest priority)
    if (name == q) score += 100;
    // Exact match on user ID
    else if (uid == q) score += 90;
    // Display name starts with query
    else if (name.rfind(q, 0) == 0) score += 60;
    // Display name contains all tokens
    else {
        auto tokens = tokenize(q);
        int matchCount = 0;
        for (const auto& t : tokens) {
            if (name.find(t) != std::string::npos) matchCount++;
            else if (uid.find(t) != std::string::npos) matchCount++;
        }
        if (matchCount == static_cast<int>(tokens.size())) score += 50;
        else if (matchCount > 0) score += matchCount * 15;
    }

    // Contains query as substring
    if (name.find(q) != std::string::npos) score += 20;
    if (uid.find(q) != std::string::npos) score += 10;

    return score;
}

// ====== Sorting ======

void UserDirectoryManager::sortByRelevance(std::vector<UserSearchResult>& results, const std::string& query) const {
    std::sort(results.begin(), results.end(), [](const UserSearchResult& a, const UserSearchResult& b) {
        // Higher score first
        if (a.relevanceScore != b.relevanceScore) return a.relevanceScore > b.relevanceScore;
        // Then alphabetical by display name
        if (a.displayName != b.displayName) return a.displayName < b.displayName;
        // Then by user ID
        return a.userId < b.userId;
    });
}

// ====== Display Formatting ======

std::string UserDirectoryManager::getBestDisplayName(const UserSearchResult& user) const {
    return getBestDisplayName(user.displayName, user.userId);
}

std::string UserDirectoryManager::getBestDisplayName(const std::string& displayName, const std::string& userId) const {
    if (!displayName.empty()) return displayName;
    // Extract localpart from @user:server
    if (userId.size() > 1 && userId[0] == '@') {
        auto colon = userId.find(':');
        if (colon != std::string::npos) return userId.substr(1, colon - 1);
    }
    return userId;
}

std::string UserDirectoryManager::formatUserResult(const UserSearchResult& user) const {
    auto name = getBestDisplayName(user);
    return name + " (" + user.userId + ")";
}

std::string UserDirectoryManager::getAvatarInitial(const UserSearchResult& user) const {
    auto name = getBestDisplayName(user);
    if (!name.empty()) return name.substr(0, 1);
    if (user.userId.size() > 1 && user.userId[0] == '@') return user.userId.substr(1, 1);
    return "?";
}

// ====== Validation ======

bool UserDirectoryManager::isValidSearchQuery(const std::string& query) const {
    return query.size() >= 2 && query.size() <= 256;
}

// ====== Serialization ======

std::string UserDirectoryManager::resultsToJson(const std::vector<UserSearchResult>& results) const {
    std::ostringstream os;
    os << "[";
    for (size_t i = 0; i < results.size(); i++) {
        if (i > 0) os << ",";
        os << userToJson(results[i]);
    }
    os << "]";
    return os.str();
}

std::string UserDirectoryManager::userToJson(const UserSearchResult& user) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"user_id":")" << esc(user.userId)
       << R"(","display_name":")" << esc(user.displayName)
       << R"(","best_name":")" << esc(getBestDisplayName(user))
       << R"(","avatar_url":")" << esc(user.avatarUrl)
       << R"(","avatar_init":")" << esc(getAvatarInitial(user))
       << R"(","relevance":)" << user.relevanceScore
       << "}";
    return os.str();
}

std::string UserDirectoryManager::responseToJson(const UserSearchResponse& response) const {
    std::ostringstream os;
    os << R"({"results":)" << resultsToJson(response.results)
       << R"(,"limited":)" << (response.limited ? "true" : "false")
       << R"(,"total":)" << response.totalResults;
    if (!response.error.empty()) os << R"(,"error":")" << response.error << R"(")";
    os << "}";
    return os.str();
}

} // namespace progressive
