#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LocationShareState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* location_share_stateToString(LocationShareState s);
LocationShareState location_share_stateFromString(const std::string& s);

struct LocationShareConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LocationShareRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LocationShareResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LocationShareStatus {
    LocationShareState state = LocationShareState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LocationShareStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the location share module with configuration
bool location_share_init(const LocationShareConfig& config);

// Handle a location share request, returning a response
LocationShareResponse location_share_handle(const LocationShareRequest& req);

// Get the current operational status
LocationShareStatus location_share_getStatus();

// Reset the module to initial state
void location_share_reset();

// Validate input before processing
bool location_share_validate(const std::string& input);

// Process input and return result
std::string location_share_process(const std::string& input);

// Serialize to JSON format
json location_share_toJson(const std::string& input);

// Get runtime statistics
LocationShareStats location_share_getStats();

// Build config from JSON
LocationShareConfig location_share_configFromJson(const json& j);

// Serialize config to JSON
json location_share_configToJson(const LocationShareConfig& config);

} // namespace pcp
