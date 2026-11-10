#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ExportUtilsStatus {
    UNKNOWN = 0,
    VALID = 1,
    EXPIRED = 2,
    REVOKED = 3,
    PENDING = 4
};

const char* key_export_utils_stateToString(ExportUtilsStatus s);
ExportUtilsStatus key_export_utils_stateFromString(const std::string& s);

struct KeyExportUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct KeyExportUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct KeyExportUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct KeyExportUtilsStatus {
    ExportUtilsStatus state = ExportUtilsStatus::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct KeyExportUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the key export utils module with configuration
bool key_export_utils_init(const KeyExportUtilsConfig& config);

// Handle a key export utils request, returning a response
KeyExportUtilsResponse key_export_utils_handle(const KeyExportUtilsRequest& req);

// Get the current operational status
KeyExportUtilsStatus key_export_utils_getStatus();

// Reset the module to initial state
void key_export_utils_reset();

// Validate input before processing
bool key_export_utils_validate(const std::string& input);

// Process input and return result
std::string key_export_utils_process(const std::string& input);

// Serialize to JSON format
json key_export_utils_toJson(const std::string& input);

// Get runtime statistics
KeyExportUtilsStats key_export_utils_getStats();

// Build config from JSON
KeyExportUtilsConfig key_export_utils_configFromJson(const json& j);

// Serialize config to JSON
json key_export_utils_configToJson(const KeyExportUtilsConfig& config);

} // namespace pcp
