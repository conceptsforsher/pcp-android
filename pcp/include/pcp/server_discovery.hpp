#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DiscoveryStatus {
    UNKNOWN = 0,
    AVAILABLE = 1,
    DEGRADED = 2,
    UNAVAILABLE = 3,
    MAINTENANCE = 4
};

const char* server_discovery_stateToString(DiscoveryStatus s);
DiscoveryStatus server_discovery_stateFromString(const std::string& s);

struct ServerDiscoveryConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ServerDiscoveryRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ServerDiscoveryResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ServerDiscoveryStatus {
    DiscoveryStatus state = DiscoveryStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ServerDiscoveryStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the server discovery module with configuration
bool server_discovery_init(const ServerDiscoveryConfig& config);

// Handle a server discovery request, returning a response
ServerDiscoveryResponse server_discovery_handle(const ServerDiscoveryRequest& req);

// Get the current operational status
ServerDiscoveryStatus server_discovery_getStatus();

// Reset the module to initial state
void server_discovery_reset();

// Validate input before processing
bool server_discovery_validate(const std::string& input);

// Process input and return result
std::string server_discovery_process(const std::string& input);

// Serialize to JSON format
json server_discovery_toJson(const std::string& input);

// Get runtime statistics
ServerDiscoveryStats server_discovery_getStats();

// Build config from JSON
ServerDiscoveryConfig server_discovery_configFromJson(const json& j);

// Serialize config to JSON
json server_discovery_configToJson(const ServerDiscoveryConfig& config);

} // namespace pcp
