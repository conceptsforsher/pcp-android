#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class NotifModeState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* notif_mode_stateToString(NotifModeState s);
NotifModeState notif_mode_stateFromString(const std::string& s);

struct NotifModeConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct NotifModeRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct NotifModeResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct NotifModeStatus {
    NotifModeState state = NotifModeState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct NotifModeStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the notif mode module with configuration
bool notif_mode_init(const NotifModeConfig& config);

// Handle a notif mode request, returning a response
NotifModeResponse notif_mode_handle(const NotifModeRequest& req);

// Get the current operational status
NotifModeStatus notif_mode_getStatus();

// Reset the module to initial state
void notif_mode_reset();

// Validate input before processing
bool notif_mode_validate(const std::string& input);

// Process input and return result
std::string notif_mode_process(const std::string& input);

// Serialize to JSON format
json notif_mode_toJson(const std::string& input);

// Get runtime statistics
NotifModeStats notif_mode_getStats();

// Build config from JSON
NotifModeConfig notif_mode_configFromJson(const json& j);

// Serialize config to JSON
json notif_mode_configToJson(const NotifModeConfig& config);

} // namespace pcp
