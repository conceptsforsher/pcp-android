#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReminderState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_reminder_stateToString(ReminderState s);
ReminderState event_reminder_stateFromString(const std::string& s);

struct EventReminderConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventReminderRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventReminderResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventReminderStatus {
    ReminderState state = ReminderState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventReminderStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event reminder module with configuration
bool event_reminder_init(const EventReminderConfig& config);

// Handle a event reminder request, returning a response
EventReminderResponse event_reminder_handle(const EventReminderRequest& req);

// Get the current operational status
EventReminderStatus event_reminder_getStatus();

// Reset the module to initial state
void event_reminder_reset();

// Validate input before processing
bool event_reminder_validate(const std::string& input);

// Process input and return result
std::string event_reminder_process(const std::string& input);

// Serialize to JSON format
json event_reminder_toJson(const std::string& input);

// Get runtime statistics
EventReminderStats event_reminder_getStats();

// Build config from JSON
EventReminderConfig event_reminder_configFromJson(const json& j);

// Serialize config to JSON
json event_reminder_configToJson(const EventReminderConfig& config);

} // namespace pcp
