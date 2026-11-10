#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

enum class ContactUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* contact_utils_stateToString(ContactUtilsState s);
ContactUtilsState contact_utils_stateFromString(const std::string& s);

struct ContactUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ContactUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ContactUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ContactUtilsStatus {
    ContactUtilsState state = ContactUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ContactUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the contact utils module with configuration
bool contact_utils_init(const ContactUtilsConfig& config);

// Handle a contact utils request, returning a response
ContactUtilsResponse contact_utils_handle(const ContactUtilsRequest& req);

// Get the current operational status
ContactUtilsStatus contact_utils_getStatus();

// Reset the module to initial state
void contact_utils_reset();

// Validate input before processing
bool contact_utils_validate(const std::string& input);

// Process input and return result
std::string contact_utils_process(const std::string& input);

// Serialize to JSON format
json contact_utils_toJson(const std::string& input);

// Get runtime statistics
ContactUtilsStats contact_utils_getStats();

// Build config from JSON
ContactUtilsConfig contact_utils_configFromJson(const json& j);

// Serialize config to JSON
json contact_utils_configToJson(const ContactUtilsConfig& config);
