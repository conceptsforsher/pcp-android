#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class DeliveryUtilsState {
    UNKNOWN = 0,
    PENDING = 1,
    PROCESSING = 2,
    COMPLETE = 3,
    FAILED = 4
};

const char* event_delivery_utils_stateToString(DeliveryUtilsState s);
DeliveryUtilsState event_delivery_utils_stateFromString(const std::string& s);

struct EventDeliveryUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EventDeliveryUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EventDeliveryUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EventDeliveryUtilsStatus {
    DeliveryUtilsState state = DeliveryUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EventDeliveryUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the event delivery utils module with configuration
bool event_delivery_utils_init(const EventDeliveryUtilsConfig& config);

// Handle a event delivery utils request, returning a response
EventDeliveryUtilsResponse event_delivery_utils_handle(const EventDeliveryUtilsRequest& req);

// Get the current operational status
EventDeliveryUtilsStatus event_delivery_utils_getStatus();

// Reset the module to initial state
void event_delivery_utils_reset();

// Validate input before processing
bool event_delivery_utils_validate(const std::string& input);

// Process input and return result
std::string event_delivery_utils_process(const std::string& input);

// Serialize to JSON format
json event_delivery_utils_toJson(const std::string& input);

// Get runtime statistics
EventDeliveryUtilsStats event_delivery_utils_getStats();

// Build config from JSON
EventDeliveryUtilsConfig event_delivery_utils_configFromJson(const json& j);

// Serialize config to JSON
json event_delivery_utils_configToJson(const EventDeliveryUtilsConfig& config);
