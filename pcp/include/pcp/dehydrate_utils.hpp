#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DehydrateUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* dehydrate_utils_stateToString(DehydrateUtilsState s);
DehydrateUtilsState dehydrate_utils_stateFromString(const std::string& s);

struct DehydrateUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DehydrateUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DehydrateUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DehydrateUtilsStatus {
    DehydrateUtilsState state = DehydrateUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DehydrateUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the dehydrate utils module with configuration
bool dehydrate_utils_init(const DehydrateUtilsConfig& config);

// Handle a dehydrate utils request, returning a response
DehydrateUtilsResponse dehydrate_utils_handle(const DehydrateUtilsRequest& req);

// Get the current operational status
DehydrateUtilsStatus dehydrate_utils_getStatus();

// Reset the module to initial state
void dehydrate_utils_reset();

// Validate input before processing
bool dehydrate_utils_validate(const std::string& input);

// Process input and return result
std::string dehydrate_utils_process(const std::string& input);

// Serialize to JSON format
json dehydrate_utils_toJson(const std::string& input);

// Get runtime statistics
DehydrateUtilsStats dehydrate_utils_getStats();

// Build config from JSON
DehydrateUtilsConfig dehydrate_utils_configFromJson(const json& j);

// Serialize config to JSON
json dehydrate_utils_configToJson(const DehydrateUtilsConfig& config);

} // namespace pcp
