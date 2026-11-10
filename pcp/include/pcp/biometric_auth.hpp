#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class BiometricAuthState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* biometric_auth_stateToString(BiometricAuthState s);
BiometricAuthState biometric_auth_stateFromString(const std::string& s);

struct BiometricAuthConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct BiometricAuthRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct BiometricAuthResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct BiometricAuthStatus {
    BiometricAuthState state = BiometricAuthState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct BiometricAuthStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the biometric auth module with configuration
bool biometric_auth_init(const BiometricAuthConfig& config);

// Handle a biometric auth request, returning a response
BiometricAuthResponse biometric_auth_handle(const BiometricAuthRequest& req);

// Get the current operational status
BiometricAuthStatus biometric_auth_getStatus();

// Reset the module to initial state
void biometric_auth_reset();

// Validate input before processing
bool biometric_auth_validate(const std::string& input);

// Process input and return result
std::string biometric_auth_process(const std::string& input);

// Serialize to JSON format
json biometric_auth_toJson(const std::string& input);

// Get runtime statistics
BiometricAuthStats biometric_auth_getStats();

// Build config from JSON
BiometricAuthConfig biometric_auth_configFromJson(const json& j);

// Serialize config to JSON
json biometric_auth_configToJson(const BiometricAuthConfig& config);

} // namespace pcp
