#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class RedactionState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_redaction_stateToString(RedactionState s);
RedactionState event_redaction_stateFromString(const std::string& s);

struct EventRedactionConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventRedactionRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventRedactionResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventRedactionStatus {
    RedactionState state = RedactionState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventRedactionStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event redaction module with configuration
bool event_redaction_init(const EventRedactionConfig& config);

// Handle a event redaction request, returning a response
EventRedactionResponse event_redaction_handle(const EventRedactionRequest& req);

// Get the current operational status
EventRedactionStatus event_redaction_getStatus();

// Reset the module to initial state
void event_redaction_reset();

// Validate input before processing
bool event_redaction_validate(const std::string& input);

// Process input and return result
std::string event_redaction_process(const std::string& input);

// Serialize to JSON format
json event_redaction_toJson(const std::string& input);

// Get runtime statistics
EventRedactionStats event_redaction_getStats();

// Build config from JSON
EventRedactionConfig event_redaction_configFromJson(const json& j);

// Serialize config to JSON
json event_redaction_configToJson(const EventRedactionConfig& config);

} // namespace pcp
