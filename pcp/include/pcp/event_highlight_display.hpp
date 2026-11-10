#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class HighlightDisplayState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_highlight_display_stateToString(HighlightDisplayState s);
HighlightDisplayState event_highlight_display_stateFromString(const std::string& s);

struct EventHighlightDisplayConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventHighlightDisplayRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventHighlightDisplayResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventHighlightDisplayStatus {
    HighlightDisplayState state = HighlightDisplayState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventHighlightDisplayStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event highlight display module with configuration
bool event_highlight_display_init(const EventHighlightDisplayConfig& config);

// Handle a event highlight display request, returning a response
EventHighlightDisplayResponse event_highlight_display_handle(const EventHighlightDisplayRequest& req);

// Get the current operational status
EventHighlightDisplayStatus event_highlight_display_getStatus();

// Reset the module to initial state
void event_highlight_display_reset();

// Validate input before processing
bool event_highlight_display_validate(const std::string& input);

// Process input and return result
std::string event_highlight_display_process(const std::string& input);

// Serialize to JSON format
json event_highlight_display_toJson(const std::string& input);

// Get runtime statistics
EventHighlightDisplayStats event_highlight_display_getStats();

// Build config from JSON
EventHighlightDisplayConfig event_highlight_display_configFromJson(const json& j);

// Serialize config to JSON
json event_highlight_display_configToJson(const EventHighlightDisplayConfig& config);
