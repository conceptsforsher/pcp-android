#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PaginationState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_pagination_stateToString(PaginationState s);
PaginationState event_pagination_stateFromString(const std::string& s);

struct EventPaginationConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventPaginationRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventPaginationResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventPaginationStatus {
    PaginationState state = PaginationState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventPaginationStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event pagination module with configuration
bool event_pagination_init(const EventPaginationConfig& config);

// Handle a event pagination request, returning a response
EventPaginationResponse event_pagination_handle(const EventPaginationRequest& req);

// Get the current operational status
EventPaginationStatus event_pagination_getStatus();

// Reset the module to initial state
void event_pagination_reset();

// Validate input before processing
bool event_pagination_validate(const std::string& input);

// Process input and return result
std::string event_pagination_process(const std::string& input);

// Serialize to JSON format
json event_pagination_toJson(const std::string& input);

// Get runtime statistics
EventPaginationStats event_pagination_getStats();

// Build config from JSON
EventPaginationConfig event_pagination_configFromJson(const json& j);

// Serialize config to JSON
json event_pagination_configToJson(const EventPaginationConfig& config);

} // namespace pcp
