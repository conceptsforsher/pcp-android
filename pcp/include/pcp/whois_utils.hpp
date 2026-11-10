#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class WhoisUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* whois_utils_stateToString(WhoisUtilsState s);
WhoisUtilsState whois_utils_stateFromString(const std::string& s);

struct WhoisUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WhoisUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WhoisUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WhoisUtilsStatus {
    WhoisUtilsState state = WhoisUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WhoisUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the whois utils module with configuration
bool whois_utils_init(const WhoisUtilsConfig& config);

// Handle a whois utils request, returning a response
WhoisUtilsResponse whois_utils_handle(const WhoisUtilsRequest& req);

// Get the current operational status
WhoisUtilsStatus whois_utils_getStatus();

// Reset the module to initial state
void whois_utils_reset();

// Validate input before processing
bool whois_utils_validate(const std::string& input);

// Process input and return result
std::string whois_utils_process(const std::string& input);

// Serialize to JSON format
json whois_utils_toJson(const std::string& input);

// Get runtime statistics
WhoisUtilsStats whois_utils_getStats();

// Build config from JSON
WhoisUtilsConfig whois_utils_configFromJson(const json& j);

// Serialize config to JSON
json whois_utils_configToJson(const WhoisUtilsConfig& config);
