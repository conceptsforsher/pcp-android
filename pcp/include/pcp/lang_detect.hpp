#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LangDetectState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* lang_detect_stateToString(LangDetectState s);
LangDetectState lang_detect_stateFromString(const std::string& s);

struct LangDetectConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LangDetectRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LangDetectResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LangDetectStatus {
    LangDetectState state = LangDetectState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LangDetectStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the lang detect module with configuration
bool lang_detect_init(const LangDetectConfig& config);

// Handle a lang detect request, returning a response
LangDetectResponse lang_detect_handle(const LangDetectRequest& req);

// Get the current operational status
LangDetectStatus lang_detect_getStatus();

// Reset the module to initial state
void lang_detect_reset();

// Validate input before processing
bool lang_detect_validate(const std::string& input);

// Process input and return result
std::string lang_detect_process(const std::string& input);

// Serialize to JSON format
json lang_detect_toJson(const std::string& input);

// Get runtime statistics
LangDetectStats lang_detect_getStats();

// Build config from JSON
LangDetectConfig lang_detect_configFromJson(const json& j);

// Serialize config to JSON
json lang_detect_configToJson(const LangDetectConfig& config);

} // namespace pcp
