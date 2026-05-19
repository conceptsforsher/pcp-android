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

static int64_t extractInt(const std::string& json, const std::string& key, int64_t def = 0) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return def;
    pp = json.find(':', pp + key.size() + 2);
    if (pp == std::string::npos) return def;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\n')) pp++;
    std::string num;
    while (pp < json.size() && (std::isdigit(json[pp]) || json[pp] == '-')) {
        num += json[pp];
        pp++;
    }
    if (num.empty()) return def;
    return std::stoll(num);
}

static std::string escJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"' || c == '\\') out += '\\';
        out += c;
    }
    return out;
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

    // Parse next_batch if present
    resp.nextBatch = extractStr(json, "next_batch");

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
    std::ostringstream os;
    os << R"({"user_id":")" << escJson(user.userId)
       << R"(","display_name":")" << escJson(user.displayName)
       << R"(","best_name":")" << escJson(getBestDisplayName(user))
       << R"(","avatar_url":")" << escJson(user.avatarUrl)
       << R"(","avatar_init":")" << escJson(getAvatarInitial(user))
       << R"(","relevance":)" << user.relevanceScore
       << "}";
    return os.str();
}

std::string UserDirectoryManager::responseToJson(const UserSearchResponse& response) const {
    std::ostringstream os;
    os << R"({"results":)" << resultsToJson(response.results)
       << R"(,"limited":)" << (response.limited ? "true" : "false")
       << R"(,"total":)" << response.totalResults;
    if (!response.nextBatch.empty()) os << R"(,"next_batch":")" << response.nextBatch << R"(")";
    if (!response.error.empty()) os << R"(,"error":")" << response.error << R"(")";
    os << "}";
    return os.str();
}

// ================================================================
// Free-standing User Search functions
// ================================================================

// Original Kotlin: SearchUsersParams.kt
std::string buildUserSearchRequest(const std::string& searchTerm, int limit) {
    std::ostringstream os;
    os << R"({"search_term":")" << searchTerm
       << R"(","limit":)" << limit
       << "}";
    return os.str();
}

// Original Kotlin: SearchUsersResponse.kt + SearchUser.kt
UserSearchResponse parseUserSearchResponse(const std::string& json) {
    UserSearchResponse resp;

    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        resp.error = err + ": " + extractStr(json, "error");
        return resp;
    }

    resp.limited = extractBool(json, "limited");
    resp.nextBatch = extractStr(json, "next_batch");

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

// ================================================================
// User Profile functions
// ================================================================

// Original Kotlin: ProfileService.Constants DISPLAY_NAME_KEY = "displayname"
std::string buildUserProfileDisplayNameUpdate(const std::string& displayName) {
    // Original Kotlin: PUT /profile/{userId}/displayname body
    std::ostringstream os;
    os << R"({"displayname":")" << escJson(displayName) << R"("})";
    return os.str();
}

// Original Kotlin: ProfileService.Constants AVATAR_URL_KEY = "avatar_url"
std::string buildUserProfileAvatarUpdate(const std::string& avatarUrl) {
    // Original Kotlin: PUT /profile/{userId}/avatar_url body
    std::ostringstream os;
    os << R"({"avatar_url":")" << escJson(avatarUrl) << R"("})";
    return os.str();
}

// Original Kotlin: User.kt fromJson(), ProfileService.getProfile()
UserProfile parseUserProfile(const std::string& userId, const std::string& json) {
    // Original Kotlin: ProfileService.Constants.DISPLAY_NAME_KEY = "displayname"
    // Original Kotlin: ProfileService.Constants.AVATAR_URL_KEY = "avatar_url"
    UserProfile profile;
    profile.userId = userId;
    profile.displayName = extractStr(json, "displayname");
    profile.avatarUrl = extractStr(json, "avatar_url");
    return profile;
}

// ================================================================
// User Status / Presence functions
// ================================================================

// Original Kotlin: SetPresenceTask body {"presence":"online","status_msg":"..."}
std::string userStatusToJson(const UserStatus& status) {
    std::ostringstream os;
    os << R"({"presence":")" << escJson(status.status)
       << R"(","status_msg":")" << escJson(status.message)
       << R"("})";
    return os.str();
}

// Original Kotlin: GET /presence/{userId}/status response
//   {"presence":"online","status_msg":"Working","currently_active":true,"last_active_ago":1000}
UserStatus parseUserStatus(const std::string& userId, const std::string& json) {
    UserStatus status;
    status.userId = userId;
    status.status = extractStr(json, "presence");
    status.message = extractStr(json, "status_msg");
    status.currentlyActive = extractBool(json, "currently_active");
    status.lastActiveAgo = extractInt(json, "last_active_ago");
    return status;
}

// ================================================================
// Expanded User Search — filter, options, pagination, batch profiles
// ================================================================

// Original Kotlin: UserDirectoryService.searchUsers(filter, options)
std::string buildUserSearchRequest(const UserSearchFilter& filter, const UserSearchOptions& options) {
    std::ostringstream os;
    os << "{";
    os << R"("search_term":")" << escJson(filter.searchTerm) << R"(")";

    if (filter.limit > 0) {
        os << R"(,"limit":)" << filter.limit;
    }

    if (!filter.excludeUserIds.empty()) {
        os << R"(,"exclude_user_ids":[)";
        for (size_t i = 0; i < filter.excludeUserIds.size(); i++) {
            if (i > 0) os << ",";
            os << R"(")" << escJson(filter.excludeUserIds[i]) << R"(")";
        }
        os << "]";
    }

    if (!filter.excludeRoomIds.empty()) {
        os << R"(,"exclude_room_ids":[)";
        for (size_t i = 0; i < filter.excludeRoomIds.size(); i++) {
            if (i > 0) os << ",";
            os << R"(")" << escJson(filter.excludeRoomIds[i]) << R"(")";
        }
        os << "]";
    }

    os << R"(,"include_display_names":)" << (filter.includeDisplayNames ? "true" : "false");
    os << R"(,"include_avatars":)" << (filter.includeAvatars ? "true" : "false");

    if (filter.onlyJoinedRoom) {
        os << R"(,"only_joined_room":true)";
    }

    if (!filter.roomId.empty()) {
        os << R"(,"room_id":")" << escJson(filter.roomId) << R"(")";
    }

    if (options.orderBy == UserSearchOrderBy::NAME) {
        os << R"(,"order_by":"name")";
    } else {
        os << R"(,"order_by":"relevance")";
    }

    if (options.caseSensitive) {
        os << R"(,"case_sensitive":true)";
    }

    os << "}";
    return os.str();
}

// Original Kotlin: get configured ID server from session
UserDirectoryServer getConfiguredUserDirectory(const std::string& serverName, bool isAvailable) {
    // Original Kotlin: ID server configuration from session params
    UserDirectoryServer server;
    server.serverName = serverName;
    server.isAvailable = isAvailable;
    server.lastChecked = isAvailable ? 1 : 0; // placeholder — real impl uses time()
    return server;
}

// Original Kotlin: parse search response pagination fields
UserSearchPagination parseUserSearchPagination(const std::string& json) {
    // Original Kotlin: extract total_results, returned_results, limited, next_batch
    UserSearchPagination pagination;
    pagination.hasMore = extractBool(json, "limited");
    pagination.nextBatch = extractStr(json, "next_batch");
    pagination.totalResults = static_cast<int>(extractInt(json, "total_results", 0));
    pagination.returnedResults = static_cast<int>(extractInt(json, "returned_results", 0));

    // Fallback: count results array length
    if (pagination.returnedResults == 0) {
        size_t pos = json.find("\"results\"");
        if (pos != std::string::npos) {
            pos = json.find('[', pos);
            if (pos != std::string::npos) {
                int count = 0;
                size_t scan = pos + 1;
                while (scan < json.size()) {
                    while (scan < json.size() && (json[scan] == ' ' || json[scan] == ',' || json[scan] == '\n')) scan++;
                    if (scan >= json.size() || json[scan] == ']') break;
                    size_t objStart = scan;
                    int depth = 0;
                    while (scan < json.size()) {
                        if (json[scan] == '{') depth++;
                        else if (json[scan] == '}') depth--;
                        if (depth == 0 && json[scan] == '}') { scan++; break; }
                        scan++;
                    }
                    count++;
                }
                pagination.returnedResults = count;
            }
        }
    }

    return pagination;
}

// Original Kotlin: ProfileService.fetchUserProfilesBatch(userIds)
std::string fetchUserProfiles(const std::vector<std::string>& userIds) {
    // Original Kotlin: POST /profile/batch or equivalent endpoint
    std::ostringstream os;
    os << R"({"user_ids":[)";
    for (size_t i = 0; i < userIds.size(); i++) {
        if (i > 0) os << ",";
        os << R"(")" << escJson(userIds[i]) << R"(")";
    }
    os << "]}";
    return os.str();
}

// Original Kotlin: parse batch profile response
UserProfileBatch parseUserProfileBatch(const std::string& json) {
    // Original Kotlin: parse {"profiles":{...}, "failures":[...]} response
    UserProfileBatch batch;

    // Parse profiles map
    size_t profilesPos = json.find("\"profiles\"");
    if (profilesPos != std::string::npos) {
        profilesPos = json.find('{', profilesPos);
        if (profilesPos != std::string::npos) {
            size_t pos = profilesPos + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n')) pos++;
                if (pos >= json.size() || json[pos] == '}') break;

                if (json[pos] == '"') {
                    pos++;
                    size_t keyEnd = pos;
                    while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
                    std::string userId = json.substr(pos, keyEnd - pos);
                    pos = keyEnd + 1;

                    pos = json.find('{', pos);
                    if (pos == std::string::npos) break;
                    int depth = 0;
                    size_t objStart = pos;
                    while (pos < json.size()) {
                        if (json[pos] == '{') depth++;
                        else if (json[pos] == '}') depth--;
                        if (depth == 0 && json[pos] == '}') { pos++; break; }
                        pos++;
                    }
                    std::string profileJson = json.substr(objStart, pos - objStart);

                    UserProfile profile;
                    profile.userId = userId;
                    profile.displayName = extractStr(profileJson, "displayname");
                    profile.avatarUrl = extractStr(profileJson, "avatar_url");
                    batch.profiles[userId] = profile;
                } else {
                    pos++;
                }
            }
        }
    }

    // Parse failures array
    size_t failuresPos = json.find("\"failures\"");
    if (failuresPos != std::string::npos) {
        failuresPos = json.find('[', failuresPos);
        if (failuresPos != std::string::npos) {
            failuresPos++;
            while (failuresPos < json.size()) {
                while (failuresPos < json.size() && (json[failuresPos] == ' ' || json[failuresPos] == ',' || json[failuresPos] == '\n')) failuresPos++;
                if (failuresPos >= json.size() || json[failuresPos] == ']') break;
                if (json[failuresPos] == '"') {
                    failuresPos++;
                    size_t end = failuresPos;
                    while (end < json.size() && json[end] != '"') end++;
                    batch.failedIds.push_back(json.substr(failuresPos, end - failuresPos));
                    failuresPos = end + 1;
                } else {
                    failuresPos++;
                }
            }
        }
    }

    return batch;
}

// ================================================================
// User Display Name Cache
// ================================================================

// Original Kotlin: UserDisplayNameCache — singleton pattern via static
static UserDisplayNameCache g_displayNameCache;

UserDisplayNameCache::UserDisplayNameCache() {}

std::string UserDisplayNameCache::getCachedDisplayName(const std::string& userId) const {
    auto it = cache_.find(userId);
    if (it != cache_.end()) return it->second;
    return "";
}

void UserDisplayNameCache::setCachedDisplayName(const std::string& userId, const std::string& displayName) {
    cache_[userId] = displayName;
}

void UserDisplayNameCache::invalidateCache() {
    cache_.clear();
}

void UserDisplayNameCache::invalidateUser(const std::string& userId) {
    cache_.erase(userId);
}

bool UserDisplayNameCache::isCached(const std::string& userId) const {
    return cache_.find(userId) != cache_.end();
}

// Original Kotlin: global cache accessors
std::string getCachedDisplayName(const std::string& userId) {
    return g_displayNameCache.getCachedDisplayName(userId);
}

void setCachedDisplayName(const std::string& userId, const std::string& displayName) {
    g_displayNameCache.setCachedDisplayName(userId, displayName);
}

void invalidateCache() {
    g_displayNameCache.invalidateCache();
}

} // namespace progressive
