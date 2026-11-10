#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DirectoryFormatterState {
    UNKNOWN = 0,
    OFFLINE = 1,
    ONLINE = 2,
    UNAVAILABLE = 3,
    BLOCKED = 4
};

const char* user_directory_formatter_stateToString(DirectoryFormatterState s);
DirectoryFormatterState user_directory_formatter_stateFromString(const std::string& s);

struct UserDirectoryFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UserDirectoryFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UserDirectoryFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UserDirectoryFormatterStatus {
    DirectoryFormatterState state = DirectoryFormatterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UserDirectoryFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the user directory formatter module with configuration
bool user_directory_formatter_init(const UserDirectoryFormatterConfig& config);

// Handle a user directory formatter request, returning a response
UserDirectoryFormatterResponse user_directory_formatter_handle(const UserDirectoryFormatterRequest& req);

// Get the current operational status
UserDirectoryFormatterStatus user_directory_formatter_getStatus();

// Reset the module to initial state
void user_directory_formatter_reset();

// Validate input before processing
bool user_directory_formatter_validate(const std::string& input);

// Process input and return result
std::string user_directory_formatter_process(const std::string& input);

// Serialize to JSON format
json user_directory_formatter_toJson(const std::string& input);

// Get runtime statistics
UserDirectoryFormatterStats user_directory_formatter_getStats();

// Build config from JSON
UserDirectoryFormatterConfig user_directory_formatter_configFromJson(const json& j);

// Serialize config to JSON
json user_directory_formatter_configToJson(const UserDirectoryFormatterConfig& config);

} // namespace pcp
