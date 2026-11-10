#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FederationTesterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* federation_tester_stateToString(FederationTesterState s);
FederationTesterState federation_tester_stateFromString(const std::string& s);

struct FederationTesterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct FederationTesterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct FederationTesterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct FederationTesterStatus {
    FederationTesterState state = FederationTesterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct FederationTesterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the federation tester module with configuration
bool federation_tester_init(const FederationTesterConfig& config);

// Handle a federation tester request, returning a response
FederationTesterResponse federation_tester_handle(const FederationTesterRequest& req);

// Get the current operational status
FederationTesterStatus federation_tester_getStatus();

// Reset the module to initial state
void federation_tester_reset();

// Validate input before processing
bool federation_tester_validate(const std::string& input);

// Process input and return result
std::string federation_tester_process(const std::string& input);

// Serialize to JSON format
json federation_tester_toJson(const std::string& input);

// Get runtime statistics
FederationTesterStats federation_tester_getStats();

// Build config from JSON
FederationTesterConfig federation_tester_configFromJson(const json& j);

// Serialize config to JSON
json federation_tester_configToJson(const FederationTesterConfig& config);

} // namespace pcp
