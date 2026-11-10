#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DisplayFormatterState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_display_formatter_stateToString(DisplayFormatterState s);
DisplayFormatterState event_display_formatter_stateFromString(const std::string& s);

struct EventDisplayFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventDisplayFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventDisplayFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventDisplayFormatterStatus {
    DisplayFormatterState state = DisplayFormatterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventDisplayFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event display formatter module with configuration
bool event_display_formatter_init(const EventDisplayFormatterConfig& config);

// Handle a event display formatter request, returning a response
EventDisplayFormatterResponse event_display_formatter_handle(const EventDisplayFormatterRequest& req);

// Get the current operational status
EventDisplayFormatterStatus event_display_formatter_getStatus();

// Reset the module to initial state
void event_display_formatter_reset();

// Validate input before processing
bool event_display_formatter_validate(const std::string& input);

// Process input and return result
std::string event_display_formatter_process(const std::string& input);

// Serialize to JSON format
json event_display_formatter_toJson(const std::string& input);

// Get runtime statistics
EventDisplayFormatterStats event_display_formatter_getStats();

// Build config from JSON
EventDisplayFormatterConfig event_display_formatter_configFromJson(const json& j);

// Serialize config to JSON
json event_display_formatter_configToJson(const EventDisplayFormatterConfig& config);

} // namespace pcp
