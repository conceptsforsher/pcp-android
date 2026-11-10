#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class MatrixErrorUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* matrix_error_utils_stateToString(MatrixErrorUtilsState s);
MatrixErrorUtilsState matrix_error_utils_stateFromString(const std::string& s);

struct MatrixErrorUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct MatrixErrorUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct MatrixErrorUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct MatrixErrorUtilsStatus {
    MatrixErrorUtilsState state = MatrixErrorUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct MatrixErrorUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the matrix error utils module with configuration
bool matrix_error_utils_init(const MatrixErrorUtilsConfig& config);

// Handle a matrix error utils request, returning a response
MatrixErrorUtilsResponse matrix_error_utils_handle(const MatrixErrorUtilsRequest& req);

// Get the current operational status
MatrixErrorUtilsStatus matrix_error_utils_getStatus();

// Reset the module to initial state
void matrix_error_utils_reset();

// Validate input before processing
bool matrix_error_utils_validate(const std::string& input);

// Process input and return result
std::string matrix_error_utils_process(const std::string& input);

// Serialize to JSON format
json matrix_error_utils_toJson(const std::string& input);

// Get runtime statistics
MatrixErrorUtilsStats matrix_error_utils_getStats();

// Build config from JSON
MatrixErrorUtilsConfig matrix_error_utils_configFromJson(const json& j);

// Serialize config to JSON
json matrix_error_utils_configToJson(const MatrixErrorUtilsConfig& config);

} // namespace pcp
