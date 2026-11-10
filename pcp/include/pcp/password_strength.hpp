#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class PasswordStrengthState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* password_strength_stateToString(PasswordStrengthState s);
PasswordStrengthState password_strength_stateFromString(const std::string& s);

struct PasswordStrengthConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PasswordStrengthRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PasswordStrengthResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PasswordStrengthStatus {
    PasswordStrengthState state = PasswordStrengthState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PasswordStrengthStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the password strength module with configuration
bool password_strength_init(const PasswordStrengthConfig& config);

// Handle a password strength request, returning a response
PasswordStrengthResponse password_strength_handle(const PasswordStrengthRequest& req);

// Get the current operational status
PasswordStrengthStatus password_strength_getStatus();

// Reset the module to initial state
void password_strength_reset();

// Validate input before processing
bool password_strength_validate(const std::string& input);

// Process input and return result
std::string password_strength_process(const std::string& input);

// Serialize to JSON format
json password_strength_toJson(const std::string& input);

// Get runtime statistics
PasswordStrengthStats password_strength_getStats();

// Build config from JSON
PasswordStrengthConfig password_strength_configFromJson(const json& j);

// Serialize config to JSON
json password_strength_configToJson(const PasswordStrengthConfig& config);

} // namespace pcp
