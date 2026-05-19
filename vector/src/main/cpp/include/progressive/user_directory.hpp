#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// User Directory Search — search Matrix users across federation
//
// Ported from Element Android:
//   UserListViewModel.kt, UserSearchResult.kt
//   UserDirectoryService.kt
//
// Uses Matrix API: POST /_matrix/client/r0/user_directory/search
//   Request: {"search_term": "alice", "limit": 20}
//   Response: {"results": [{"user_id":"@alice:org","display_name":"Alice","avatar_url":"mxc://..."}], "limited": false}
// ================================================================

// ---- User Search Result ----

struct UserSearchResult {
    std::string userId;              // @alice:example.org
    std::string displayName;         // "Alice Johnson"
    std::string avatarUrl;           // mxc://example.org/avatar
    std::string matrixId;            // Same as userId (alias)
    int relevanceScore = 0;          // Search relevance (0-100)
    bool isExactMatch = false;       // Query matched exactly
    bool isValid = false;
};

// ---- User Search Query ----

struct UserSearchQuery {
    std::string searchTerm;          // Text to search
    int limit = 20;                  // Max results
    std::string serverFilter;        // Limit to specific server (optional)
    bool excludeSelf = true;         // Exclude current user
    std::string currentUserId;       // Current user ID (for excluding)
};

// ---- User Search Response ----

struct UserSearchResponse {
    std::vector<UserSearchResult> results;
    bool limited = false;            // More results available
    int totalResults = 0;
    std::string nextBatch;           // Pagination token for next batch
    std::string error;
};

// ---- User Directory Manager ----

class UserDirectoryManager {
public:
    UserDirectoryManager();

    // ====== Search ======

    // Build the search request body for /user_directory/search.
    std::string buildSearchRequest(const UserSearchQuery& query) const;

    // Parse search response from /user_directory/search.
    UserSearchResponse parseSearchResponse(const std::string& json) const;

    // Search and rank results by relevance.
    UserSearchResponse search(const UserSearchQuery& query, const std::string& responseJson);

    // ====== Results Processing ======

    // Deduplicate search results by user ID.
    void deduplicate(std::vector<UserSearchResult>& results) const;

    // Sort results by relevance (exact match first, then score, then alphabetical).
    void sortByRelevance(std::vector<UserSearchResult>& results, const std::string& query) const;

    // Calculate relevance score for a result.
    int calculateRelevance(const UserSearchResult& result, const std::string& query) const;

    // ====== Display Formatting ======

    // Get the best display name (displayName or fallback to userId).
    std::string getBestDisplayName(const UserSearchResult& user) const;
    std::string getBestDisplayName(const std::string& displayName, const std::string& userId) const;

    // Format user result for display (name + MXID).
    std::string formatUserResult(const UserSearchResult& user) const;

    // Get first character for avatar placeholder.
    std::string getAvatarInitial(const UserSearchResult& user) const;

    // ====== Query Validation ======

    // Validate search query (must be at least 2 chars, max 256).
    bool isValidSearchQuery(const std::string& query) const;

    // Get minimum search query length.
    int minQueryLength() const { return 2; }

    // ====== Serialization ======

    // Export search results as JSON for UI.
    std::string resultsToJson(const std::vector<UserSearchResult>& results) const;

    // Export single user as JSON.
    std::string userToJson(const UserSearchResult& user) const;

    // Export search response as JSON.
    std::string responseToJson(const UserSearchResponse& response) const;

private:
    // Cache of recently searched users.
    std::unordered_map<std::string, UserSearchResult> cache_;

    // Tokenize search query into words.
    std::vector<std::string> tokenize(const std::string& text) const;
};

// ================================================================
// User Profile — get/set display name and avatar
//
// Ported from Element Android:
//   ProfileService.kt, User.kt
//   GetProfileInfoTask.kt
//
// Matrix API: GET /_matrix/client/r0/profile/{userId}
//   Response: {"displayname": "Alice", "avatar_url": "mxc://..."}
//
// Matrix API: PUT /_matrix/client/r0/profile/{userId}/displayname
//   Body: {"displayname": "New Name"}
//
// Matrix API: PUT /_matrix/client/r0/profile/{userId}/avatar_url
//   Body: {"avatar_url": "mxc://..."}
// ================================================================

// ---- User Profile ----

// Original Kotlin: User.kt data class User(userId, displayName, avatarUrl)
struct UserProfile {
    std::string userId;
    std::string displayName;         // "displayname" in Matrix JSON
    std::string avatarUrl;           // "avatar_url" in Matrix JSON
};

// Build JSON body for PUT /profile/{userId}/displayname.
// Original Kotlin: ProfileService.setDisplayName()
std::string buildUserProfileDisplayNameUpdate(const std::string& displayName);

// Build JSON body for PUT /profile/{userId}/avatar_url.
// Original Kotlin: ProfileService.updateAvatar()
std::string buildUserProfileAvatarUpdate(const std::string& avatarUrl);

// Parse the combined profile response from GET /profile/{userId}.
// Original Kotlin: ProfileService.getProfile() -> User.fromJson()
UserProfile parseUserProfile(const std::string& userId, const std::string& json);

// ================================================================
// User Status / Presence — online status and status message
//
// Ported from Element Android:
//   UserPresence.kt, SetPresenceTask.kt
//
// Matrix API: PUT /_matrix/client/r0/presence/{userId}/status
//   Body: {"presence": "online", "status_msg": "Working"}
//
// Matrix API: GET /_matrix/client/r0/presence/{userId}/status
//   Response: {"presence": "online", "status_msg": "Working",
//              "currently_active": true, "last_active_ago": 1000}
// ================================================================

// ---- User Status ----

// Original Kotlin: UserPresence model
struct UserStatus {
    std::string userId;
    std::string status;              // "online", "offline", "unavailable"
    std::string message;             // Status message
    bool currentlyActive = false;    // Currently active?
    int64_t lastActiveAgo = 0;       // Milliseconds since last activity
};

// Build JSON body for PUT /presence/{userId}/status.
// Original Kotlin: SetPresenceTask body
std::string userStatusToJson(const UserStatus& status);

// Parse presence response from GET /presence/{userId}/status.
// Original Kotlin: GetPresenceTask response
UserStatus parseUserStatus(const std::string& userId, const std::string& json);

// ================================================================
// User Search Filter — expanded filter options
//
// Original Kotlin: UserSearchFilter.kt, UserListViewModel.kt
// ================================================================

// ---- User Search Filter ----

// Original Kotlin: UserListViewModel.SearchFilter
struct UserSearchFilter {
    std::string searchTerm;
    std::vector<std::string> excludeUserIds;
    std::vector<std::string> excludeRoomIds;
    int limit = 20;
    bool includeDisplayNames = true;
    bool includeAvatars = false;
    bool onlyJoinedRoom = false;
    std::string roomId;              // context room for joined-room search
};

// ---- User Search Order By ----

// Original Kotlin: UserSearchOptions.OrderBy
enum class UserSearchOrderBy {
    NAME,
    RELEVANCE
};

// ---- User Search Options ----

// Original Kotlin: UserSearchOptions data class
struct UserSearchOptions {
    UserSearchFilter filter;
    UserSearchOrderBy orderBy = UserSearchOrderBy::RELEVANCE;
    bool caseSensitive = false;
};

// ---- User Directory Server ----

// Original Kotlin: UserDirectoryServer model
struct UserDirectoryServer {
    std::string serverName;
    bool isAvailable = false;
    int64_t lastChecked = 0;
};

// ---- User Search Pagination ----

// Original Kotlin: UserSearchPagination model
struct UserSearchPagination {
    int totalResults = 0;
    int returnedResults = 0;
    bool hasMore = false;
    std::string nextBatch;
};

// ---- User Profile Batch ----

// Original Kotlin: ProfileService batch type
struct UserProfileBatch {
    std::unordered_map<std::string, UserProfile> profiles;
    std::vector<std::string> failedIds;
};

// Build a basic search request body for /user_directory/search.
// Original Kotlin: SearchUsersParams.kt
std::string buildUserSearchRequest(const std::string& searchTerm, int limit = 20);

// Build the expanded search request body for /user_directory/search.
// Original Kotlin: UserDirectoryService.searchUsers(filter, options)
std::string buildUserSearchRequest(const UserSearchFilter& filter, const UserSearchOptions& options);

// Parse search response from /user_directory/search (free function).
// Original Kotlin: SearchUsersResponse.kt + SearchUser.kt
UserSearchResponse parseUserSearchResponse(const std::string& json);

// Get the configured user directory (identity) server.
// Original Kotlin: ID server from session configuration
UserDirectoryServer getConfiguredUserDirectory(const std::string& serverName, bool isAvailable = false);

// Parse search pagination metadata from response.
// Original Kotlin: parse search response pagination fields
UserSearchPagination parseUserSearchPagination(const std::string& json);

// Build a batch profile fetch request body (POST /profile/batch or equivalent).
// Original Kotlin: ProfileService.fetchUserProfilesBatch(userIds)
std::string fetchUserProfiles(const std::vector<std::string>& userIds);

// Parse batch profile response.
// Original Kotlin: parse batch profile response JSON
UserProfileBatch parseUserProfileBatch(const std::string& json);

// ---- User Display Name Cache ----

// Original Kotlin: UserDisplayNameCache — local cache for user display names
class UserDisplayNameCache {
public:
    UserDisplayNameCache();

    // Get cached display name for userId.
    std::string getCachedDisplayName(const std::string& userId) const;

    // Set cached display name for userId.
    void setCachedDisplayName(const std::string& userId, const std::string& displayName);

    // Clear all cached entries.
    void invalidateCache();

    // Remove a specific user from cache.
    void invalidateUser(const std::string& userId);

    // Check if a user is in the cache.
    bool isCached(const std::string& userId) const;

private:
    std::unordered_map<std::string, std::string> cache_;
};

// Convenience: get cached display name via global cache singleton.
// Original Kotlin: UserDisplayNameCache accessor
std::string getCachedDisplayName(const std::string& userId);
void setCachedDisplayName(const std::string& userId, const std::string& displayName);
void invalidateCache();

} // namespace progressive
