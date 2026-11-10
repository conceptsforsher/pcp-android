#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TranslatorKind {
    UNKNOWN = 0,
    MESSAGE = 1,
    INVITE = 2,
    CALL = 3,
    NOTIFICATION = 4
};

const char* push_translator_stateToString(TranslatorKind s);
TranslatorKind push_translator_stateFromString(const std::string& s);

struct PushTranslatorConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct PushTranslatorRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct PushTranslatorResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct PushTranslatorStatus {
    TranslatorKind state = TranslatorKind::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct PushTranslatorStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the push translator module with configuration
bool push_translator_init(const PushTranslatorConfig& config);

// Handle a push translator request, returning a response
PushTranslatorResponse push_translator_handle(const PushTranslatorRequest& req);

// Get the current operational status
PushTranslatorStatus push_translator_getStatus();

// Reset the module to initial state
void push_translator_reset();

// Validate input before processing
bool push_translator_validate(const std::string& input);

// Process input and return result
std::string push_translator_process(const std::string& input);

// Serialize to JSON format
json push_translator_toJson(const std::string& input);

// Get runtime statistics
PushTranslatorStats push_translator_getStats();

// Build config from JSON
PushTranslatorConfig push_translator_configFromJson(const json& j);

// Serialize config to JSON
json push_translator_configToJson(const PushTranslatorConfig& config);

} // namespace pcp
