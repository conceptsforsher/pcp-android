#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SenderNotifFilterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* sender_notif_filter_stateToString(SenderNotifFilterState s);
SenderNotifFilterState sender_notif_filter_stateFromString(const std::string& s);

struct SenderNotifFilterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct SenderNotifFilterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct SenderNotifFilterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct SenderNotifFilterStatus {
    SenderNotifFilterState state = SenderNotifFilterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct SenderNotifFilterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the sender notif filter module with configuration
bool sender_notif_filter_init(const SenderNotifFilterConfig& config);

// Handle a sender notif filter request, returning a response
SenderNotifFilterResponse sender_notif_filter_handle(const SenderNotifFilterRequest& req);

// Get the current operational status
SenderNotifFilterStatus sender_notif_filter_getStatus();

// Reset the module to initial state
void sender_notif_filter_reset();

// Validate input before processing
bool sender_notif_filter_validate(const std::string& input);

// Process input and return result
std::string sender_notif_filter_process(const std::string& input);

// Serialize to JSON format
json sender_notif_filter_toJson(const std::string& input);

// Get runtime statistics
SenderNotifFilterStats sender_notif_filter_getStats();

// Build config from JSON
SenderNotifFilterConfig sender_notif_filter_configFromJson(const json& j);

// Serialize config to JSON
json sender_notif_filter_configToJson(const SenderNotifFilterConfig& config);

} // namespace pcp
