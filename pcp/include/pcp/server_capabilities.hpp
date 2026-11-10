#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class CapabilitiesStatus {
    UNKNOWN = 0,
    AVAILABLE = 1,
    DEGRADED = 2,
    UNAVAILABLE = 3,
    MAINTENANCE = 4
};

const char* server_capabilities_stateToString(CapabilitiesStatus s);
CapabilitiesStatus server_capabilities_stateFromString(const std::string& s);

struct ServerCapabilitiesConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ServerCapabilitiesRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ServerCapabilitiesResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ServerCapabilitiesStatus {
    CapabilitiesStatus state = CapabilitiesStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ServerCapabilitiesStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the server capabilities module with configuration
bool server_capabilities_init(const ServerCapabilitiesConfig& config);

// Handle a server capabilities request, returning a response
ServerCapabilitiesResponse server_capabilities_handle(const ServerCapabilitiesRequest& req);

// Get the current operational status
ServerCapabilitiesStatus server_capabilities_getStatus();

// Reset the module to initial state
void server_capabilities_reset();

// Validate input before processing
bool server_capabilities_validate(const std::string& input);

// Process input and return result
std::string server_capabilities_process(const std::string& input);

// Serialize to JSON format
json server_capabilities_toJson(const std::string& input);

// Get runtime statistics
ServerCapabilitiesStats server_capabilities_getStats();

// Build config from JSON
ServerCapabilitiesConfig server_capabilities_configFromJson(const json& j);

// Serialize config to JSON
json server_capabilities_configToJson(const ServerCapabilitiesConfig& config);

} // namespace pcp
