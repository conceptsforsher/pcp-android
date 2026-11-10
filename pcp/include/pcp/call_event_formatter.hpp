#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EventFormatterPhase {
    NONE = 0,
    RINGING = 1,
    CONNECTING = 2,
    ACTIVE = 3,
    ENDING = 4
};

const char* call_event_formatter_stateToString(EventFormatterPhase s);
EventFormatterPhase call_event_formatter_stateFromString(const std::string& s);

struct CallEventFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CallEventFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CallEventFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CallEventFormatterStatus {
    EventFormatterPhase state = EventFormatterPhase::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CallEventFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the call event formatter module with configuration
bool call_event_formatter_init(const CallEventFormatterConfig& config);

// Handle a call event formatter request, returning a response
CallEventFormatterResponse call_event_formatter_handle(const CallEventFormatterRequest& req);

// Get the current operational status
CallEventFormatterStatus call_event_formatter_getStatus();

// Reset the module to initial state
void call_event_formatter_reset();

// Validate input before processing
bool call_event_formatter_validate(const std::string& input);

// Process input and return result
std::string call_event_formatter_process(const std::string& input);

// Serialize to JSON format
json call_event_formatter_toJson(const std::string& input);

// Get runtime statistics
CallEventFormatterStats call_event_formatter_getStats();

// Build config from JSON
CallEventFormatterConfig call_event_formatter_configFromJson(const json& j);

// Serialize config to JSON
json call_event_formatter_configToJson(const CallEventFormatterConfig& config);

} // namespace pcp
