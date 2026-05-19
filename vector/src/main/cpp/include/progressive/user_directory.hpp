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

} // namespace progressive
