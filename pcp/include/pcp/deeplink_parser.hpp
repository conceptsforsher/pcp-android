#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DeeplinkParserState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* deeplink_parser_stateToString(DeeplinkParserState s);
DeeplinkParserState deeplink_parser_stateFromString(const std::string& s);

struct DeeplinkParserConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct DeeplinkParserRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct DeeplinkParserResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct DeeplinkParserStatus {
    DeeplinkParserState state = DeeplinkParserState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct DeeplinkParserStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the deeplink parser module with configuration
bool deeplink_parser_init(const DeeplinkParserConfig& config);

// Handle a deeplink parser request, returning a response
DeeplinkParserResponse deeplink_parser_handle(const DeeplinkParserRequest& req);

// Get the current operational status
DeeplinkParserStatus deeplink_parser_getStatus();

// Reset the module to initial state
void deeplink_parser_reset();

// Validate input before processing
bool deeplink_parser_validate(const std::string& input);

// Process input and return result
std::string deeplink_parser_process(const std::string& input);

// Serialize to JSON format
json deeplink_parser_toJson(const std::string& input);

// Get runtime statistics
DeeplinkParserStats deeplink_parser_getStats();

// Build config from JSON
DeeplinkParserConfig deeplink_parser_configFromJson(const json& j);

// Serialize config to JSON
json deeplink_parser_configToJson(const DeeplinkParserConfig& config);

} // namespace pcp
