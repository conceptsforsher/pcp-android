#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class AcronymUtilsStatus {
    UNKNOWN = 0,
    AVAILABLE = 1,
    DEGRADED = 2,
    UNAVAILABLE = 3,
    MAINTENANCE = 4
};

const char* server_acronym_utils_stateToString(AcronymUtilsStatus s);
AcronymUtilsStatus server_acronym_utils_stateFromString(const std::string& s);

struct ServerAcronymUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ServerAcronymUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ServerAcronymUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ServerAcronymUtilsStatus {
    AcronymUtilsStatus state = AcronymUtilsStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ServerAcronymUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the server acronym utils module with configuration
bool server_acronym_utils_init(const ServerAcronymUtilsConfig& config);

// Handle a server acronym utils request, returning a response
ServerAcronymUtilsResponse server_acronym_utils_handle(const ServerAcronymUtilsRequest& req);

// Get the current operational status
ServerAcronymUtilsStatus server_acronym_utils_getStatus();

// Reset the module to initial state
void server_acronym_utils_reset();

// Validate input before processing
bool server_acronym_utils_validate(const std::string& input);

// Process input and return result
std::string server_acronym_utils_process(const std::string& input);

// Serialize to JSON format
json server_acronym_utils_toJson(const std::string& input);

// Get runtime statistics
ServerAcronymUtilsStats server_acronym_utils_getStats();

// Build config from JSON
ServerAcronymUtilsConfig server_acronym_utils_configFromJson(const json& j);

// Serialize config to JSON
json server_acronym_utils_configToJson(const ServerAcronymUtilsConfig& config);
