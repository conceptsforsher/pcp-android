#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DurationPhase {
    NONE = 0,
    RINGING = 1,
    CONNECTING = 2,
    ACTIVE = 3,
    ENDING = 4
};

const char* call_duration_stateToString(DurationPhase s);
DurationPhase call_duration_stateFromString(const std::string& s);

struct CallDurationConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CallDurationRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CallDurationResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CallDurationStatus {
    DurationPhase state = DurationPhase::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CallDurationStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the call duration module with configuration
bool call_duration_init(const CallDurationConfig& config);

// Handle a call duration request, returning a response
CallDurationResponse call_duration_handle(const CallDurationRequest& req);

// Get the current operational status
CallDurationStatus call_duration_getStatus();

// Reset the module to initial state
void call_duration_reset();

// Validate input before processing
bool call_duration_validate(const std::string& input);

// Process input and return result
std::string call_duration_process(const std::string& input);

// Serialize to JSON format
json call_duration_toJson(const std::string& input);

// Get runtime statistics
CallDurationStats call_duration_getStats();

// Build config from JSON
CallDurationConfig call_duration_configFromJson(const json& j);

// Serialize config to JSON
json call_duration_configToJson(const CallDurationConfig& config);

} // namespace pcp
