#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UrlPreviewParserState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* url_preview_parser_stateToString(UrlPreviewParserState s);
UrlPreviewParserState url_preview_parser_stateFromString(const std::string& s);

struct UrlPreviewParserConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UrlPreviewParserRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UrlPreviewParserResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UrlPreviewParserStatus {
    UrlPreviewParserState state = UrlPreviewParserState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UrlPreviewParserStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the url preview parser module with configuration
bool url_preview_parser_init(const UrlPreviewParserConfig& config);

// Handle a url preview parser request, returning a response
UrlPreviewParserResponse url_preview_parser_handle(const UrlPreviewParserRequest& req);

// Get the current operational status
UrlPreviewParserStatus url_preview_parser_getStatus();

// Reset the module to initial state
void url_preview_parser_reset();

// Validate input before processing
bool url_preview_parser_validate(const std::string& input);

// Process input and return result
std::string url_preview_parser_process(const std::string& input);

// Serialize to JSON format
json url_preview_parser_toJson(const std::string& input);

// Get runtime statistics
UrlPreviewParserStats url_preview_parser_getStats();

// Build config from JSON
UrlPreviewParserConfig url_preview_parser_configFromJson(const json& j);

// Serialize config to JSON
json url_preview_parser_configToJson(const UrlPreviewParserConfig& config);

} // namespace pcp
