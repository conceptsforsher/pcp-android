#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TypingIndicatorState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* typing_indicator_stateToString(TypingIndicatorState s);
TypingIndicatorState typing_indicator_stateFromString(const std::string& s);

struct TypingIndicatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct TypingIndicatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct TypingIndicatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct TypingIndicatorStatus {
    TypingIndicatorState state = TypingIndicatorState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct TypingIndicatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the typing indicator module with configuration
bool typing_indicator_init(const TypingIndicatorConfig& config);

// Handle a typing indicator request, returning a response
TypingIndicatorResponse typing_indicator_handle(const TypingIndicatorRequest& req);

// Get the current operational status
TypingIndicatorStatus typing_indicator_getStatus();

// Reset the module to initial state
void typing_indicator_reset();

// Validate input before processing
bool typing_indicator_validate(const std::string& input);

// Process input and return result
std::string typing_indicator_process(const std::string& input);

// Serialize to JSON format
json typing_indicator_toJson(const std::string& input);

// Get runtime statistics
TypingIndicatorStats typing_indicator_getStats();

// Build config from JSON
TypingIndicatorConfig typing_indicator_configFromJson(const json& j);

// Serialize config to JSON
json typing_indicator_configToJson(const TypingIndicatorConfig& config);

} // namespace pcp
