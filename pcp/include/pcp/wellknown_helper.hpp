#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class WellknownHelperState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* wellknown_helper_stateToString(WellknownHelperState s);
WellknownHelperState wellknown_helper_stateFromString(const std::string& s);

struct WellknownHelperConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct WellknownHelperRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct WellknownHelperResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct WellknownHelperStatus {
    WellknownHelperState state = WellknownHelperState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct WellknownHelperStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the wellknown helper module with configuration
bool wellknown_helper_init(const WellknownHelperConfig& config);

// Handle a wellknown helper request, returning a response
WellknownHelperResponse wellknown_helper_handle(const WellknownHelperRequest& req);

// Get the current operational status
WellknownHelperStatus wellknown_helper_getStatus();

// Reset the module to initial state
void wellknown_helper_reset();

// Validate input before processing
bool wellknown_helper_validate(const std::string& input);

// Process input and return result
std::string wellknown_helper_process(const std::string& input);

// Serialize to JSON format
json wellknown_helper_toJson(const std::string& input);

// Get runtime statistics
WellknownHelperStats wellknown_helper_getStats();

// Build config from JSON
WellknownHelperConfig wellknown_helper_configFromJson(const json& j);

// Serialize config to JSON
json wellknown_helper_configToJson(const WellknownHelperConfig& config);

} // namespace pcp
