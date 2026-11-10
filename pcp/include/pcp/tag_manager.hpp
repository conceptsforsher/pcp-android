#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TagManagerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* tag_manager_stateToString(TagManagerState s);
TagManagerState tag_manager_stateFromString(const std::string& s);

struct TagManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct TagManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct TagManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct TagManagerStatus {
    TagManagerState state = TagManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct TagManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the tag manager module with configuration
bool tag_manager_init(const TagManagerConfig& config);

// Handle a tag manager request, returning a response
TagManagerResponse tag_manager_handle(const TagManagerRequest& req);

// Get the current operational status
TagManagerStatus tag_manager_getStatus();

// Reset the module to initial state
void tag_manager_reset();

// Validate input before processing
bool tag_manager_validate(const std::string& input);

// Process input and return result
std::string tag_manager_process(const std::string& input);

// Serialize to JSON format
json tag_manager_toJson(const std::string& input);

// Get runtime statistics
TagManagerStats tag_manager_getStats();

// Build config from JSON
TagManagerConfig tag_manager_configFromJson(const json& j);

// Serialize config to JSON
json tag_manager_configToJson(const TagManagerConfig& config);

} // namespace pcp
