#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class LanguageSwitcherState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* language_switcher_stateToString(LanguageSwitcherState s);
LanguageSwitcherState language_switcher_stateFromString(const std::string& s);

struct LanguageSwitcherConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct LanguageSwitcherRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct LanguageSwitcherResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct LanguageSwitcherStatus {
    LanguageSwitcherState state = LanguageSwitcherState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct LanguageSwitcherStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the language switcher module with configuration
bool language_switcher_init(const LanguageSwitcherConfig& config);

// Handle a language switcher request, returning a response
LanguageSwitcherResponse language_switcher_handle(const LanguageSwitcherRequest& req);

// Get the current operational status
LanguageSwitcherStatus language_switcher_getStatus();

// Reset the module to initial state
void language_switcher_reset();

// Validate input before processing
bool language_switcher_validate(const std::string& input);

// Process input and return result
std::string language_switcher_process(const std::string& input);

// Serialize to JSON format
json language_switcher_toJson(const std::string& input);

// Get runtime statistics
LanguageSwitcherStats language_switcher_getStats();

// Build config from JSON
LanguageSwitcherConfig language_switcher_configFromJson(const json& j);

// Serialize config to JSON
json language_switcher_configToJson(const LanguageSwitcherConfig& config);

} // namespace pcp
