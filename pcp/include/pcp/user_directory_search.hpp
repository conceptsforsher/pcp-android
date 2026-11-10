#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DirectorySearchState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_directory_search_stateToString(DirectorySearchState s);
DirectorySearchState user_directory_search_stateFromString(const std::string& s);

struct UserDirectorySearchConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserDirectorySearchRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserDirectorySearchResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserDirectorySearchStatus {
    DirectorySearchState state = DirectorySearchState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserDirectorySearchStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user directory search module with configuration
bool user_directory_search_init(const UserDirectorySearchConfig& config);

// Handle a user directory search request, returning a response
UserDirectorySearchResponse user_directory_search_handle(const UserDirectorySearchRequest& req);

// Get the current operational status
UserDirectorySearchStatus user_directory_search_getStatus();

// Reset the module to initial state
void user_directory_search_reset();

// Validate input before processing
bool user_directory_search_validate(const std::string& input);

// Process input and return result
std::string user_directory_search_process(const std::string& input);

// Serialize to JSON format
json user_directory_search_toJson(const std::string& input);

// Get runtime statistics
UserDirectorySearchStats user_directory_search_getStats();

// Build config from JSON
UserDirectorySearchConfig user_directory_search_configFromJson(const json& j);

// Serialize config to JSON
json user_directory_search_configToJson(const UserDirectorySearchConfig& config);

} // namespace pcp
