#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class SupportStatus {
    UNKNOWN = 0,
    AVAILABLE = 1,
    DEGRADED = 2,
    UNAVAILABLE = 3,
    MAINTENANCE = 4
};

const char* server_support_stateToString(SupportStatus s);
SupportStatus server_support_stateFromString(const std::string& s);

struct ServerSupportConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ServerSupportRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ServerSupportResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ServerSupportStatus {
    SupportStatus state = SupportStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ServerSupportStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the server support module with configuration
bool server_support_init(const ServerSupportConfig& config);

// Handle a server support request, returning a response
ServerSupportResponse server_support_handle(const ServerSupportRequest& req);

// Get the current operational status
ServerSupportStatus server_support_getStatus();

// Reset the module to initial state
void server_support_reset();

// Validate input before processing
bool server_support_validate(const std::string& input);

// Process input and return result
std::string server_support_process(const std::string& input);

// Serialize to JSON format
json server_support_toJson(const std::string& input);

// Get runtime statistics
ServerSupportStats server_support_getStats();

// Build config from JSON
ServerSupportConfig server_support_configFromJson(const json& j);

// Serialize config to JSON
json server_support_configToJson(const ServerSupportConfig& config);

} // namespace pcp
