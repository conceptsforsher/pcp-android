#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MetadataUtilsPhase {
    INIT = 0,
    CONNECTING = 1,
    ACTIVE = 2,
    EXPIRING = 3,
    TERMINATED = 4
};

const char* session_metadata_utils_stateToString(MetadataUtilsPhase s);
MetadataUtilsPhase session_metadata_utils_stateFromString(const std::string& s);

struct SessionMetadataUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SessionMetadataUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SessionMetadataUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SessionMetadataUtilsStatus {
    MetadataUtilsPhase state = MetadataUtilsPhase::INIT;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SessionMetadataUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the session metadata utils module with configuration
bool session_metadata_utils_init(const SessionMetadataUtilsConfig& config);

// Handle a session metadata utils request, returning a response
SessionMetadataUtilsResponse session_metadata_utils_handle(const SessionMetadataUtilsRequest& req);

// Get the current operational status
SessionMetadataUtilsStatus session_metadata_utils_getStatus();

// Reset the module to initial state
void session_metadata_utils_reset();

// Validate input before processing
bool session_metadata_utils_validate(const std::string& input);

// Process input and return result
std::string session_metadata_utils_process(const std::string& input);

// Serialize to JSON format
json session_metadata_utils_toJson(const std::string& input);

// Get runtime statistics
SessionMetadataUtilsStats session_metadata_utils_getStats();

// Build config from JSON
SessionMetadataUtilsConfig session_metadata_utils_configFromJson(const json& j);

// Serialize config to JSON
json session_metadata_utils_configToJson(const SessionMetadataUtilsConfig& config);

} // namespace pcp
