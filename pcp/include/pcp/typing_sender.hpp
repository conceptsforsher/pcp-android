#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TypingSenderState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* typing_sender_stateToString(TypingSenderState s);
TypingSenderState typing_sender_stateFromString(const std::string& s);

struct TypingSenderConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct TypingSenderRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct TypingSenderResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct TypingSenderStatus {
    TypingSenderState state = TypingSenderState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct TypingSenderStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the typing sender module with configuration
bool typing_sender_init(const TypingSenderConfig& config);

// Handle a typing sender request, returning a response
TypingSenderResponse typing_sender_handle(const TypingSenderRequest& req);

// Get the current operational status
TypingSenderStatus typing_sender_getStatus();

// Reset the module to initial state
void typing_sender_reset();

// Validate input before processing
bool typing_sender_validate(const std::string& input);

// Process input and return result
std::string typing_sender_process(const std::string& input);

// Serialize to JSON format
json typing_sender_toJson(const std::string& input);

// Get runtime statistics
TypingSenderStats typing_sender_getStats();

// Build config from JSON
TypingSenderConfig typing_sender_configFromJson(const json& j);

// Serialize config to JSON
json typing_sender_configToJson(const TypingSenderConfig& config);

} // namespace pcp
