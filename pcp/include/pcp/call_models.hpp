#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ModelsPhase {
    NONE = 0,
    RINGING = 1,
    CONNECTING = 2,
    ACTIVE = 3,
    ENDING = 4
};

const char* call_models_stateToString(ModelsPhase s);
ModelsPhase call_models_stateFromString(const std::string& s);

struct CallModelsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct CallModelsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct CallModelsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct CallModelsStatus {
    ModelsPhase state = ModelsPhase::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct CallModelsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the call models module with configuration
bool call_models_init(const CallModelsConfig& config);

// Handle a call models request, returning a response
CallModelsResponse call_models_handle(const CallModelsRequest& req);

// Get the current operational status
CallModelsStatus call_models_getStatus();

// Reset the module to initial state
void call_models_reset();

// Validate input before processing
bool call_models_validate(const std::string& input);

// Process input and return result
std::string call_models_process(const std::string& input);

// Serialize to JSON format
json call_models_toJson(const std::string& input);

// Get runtime statistics
CallModelsStats call_models_getStats();

// Build config from JSON
CallModelsConfig call_models_configFromJson(const json& j);

// Serialize config to JSON
json call_models_configToJson(const CallModelsConfig& config);

} // namespace pcp
