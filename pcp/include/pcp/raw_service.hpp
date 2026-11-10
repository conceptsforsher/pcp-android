#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class RawServiceState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* raw_service_stateToString(RawServiceState s);
RawServiceState raw_service_stateFromString(const std::string& s);

struct RawServiceConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RawServiceRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RawServiceResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RawServiceStatus {
    RawServiceState state = RawServiceState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RawServiceStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the raw service module with configuration
bool raw_service_init(const RawServiceConfig& config);

// Handle a raw service request, returning a response
RawServiceResponse raw_service_handle(const RawServiceRequest& req);

// Get the current operational status
RawServiceStatus raw_service_getStatus();

// Reset the module to initial state
void raw_service_reset();

// Validate input before processing
bool raw_service_validate(const std::string& input);

// Process input and return result
std::string raw_service_process(const std::string& input);

// Serialize to JSON format
json raw_service_toJson(const std::string& input);

// Get runtime statistics
RawServiceStats raw_service_getStats();

// Build config from JSON
RawServiceConfig raw_service_configFromJson(const json& j);

// Serialize config to JSON
json raw_service_configToJson(const RawServiceConfig& config);

} // namespace pcp
