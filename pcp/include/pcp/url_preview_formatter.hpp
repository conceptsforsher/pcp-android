#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class UrlPreviewFormatterState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* url_preview_formatter_stateToString(UrlPreviewFormatterState s);
UrlPreviewFormatterState url_preview_formatter_stateFromString(const std::string& s);

struct UrlPreviewFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct UrlPreviewFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct UrlPreviewFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct UrlPreviewFormatterStatus {
    UrlPreviewFormatterState state = UrlPreviewFormatterState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct UrlPreviewFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the url preview formatter module with configuration
bool url_preview_formatter_init(const UrlPreviewFormatterConfig& config);

// Handle a url preview formatter request, returning a response
UrlPreviewFormatterResponse url_preview_formatter_handle(const UrlPreviewFormatterRequest& req);

// Get the current operational status
UrlPreviewFormatterStatus url_preview_formatter_getStatus();

// Reset the module to initial state
void url_preview_formatter_reset();

// Validate input before processing
bool url_preview_formatter_validate(const std::string& input);

// Process input and return result
std::string url_preview_formatter_process(const std::string& input);

// Serialize to JSON format
json url_preview_formatter_toJson(const std::string& input);

// Get runtime statistics
UrlPreviewFormatterStats url_preview_formatter_getStats();

// Build config from JSON
UrlPreviewFormatterConfig url_preview_formatter_configFromJson(const json& j);

// Serialize config to JSON
json url_preview_formatter_configToJson(const UrlPreviewFormatterConfig& config);

} // namespace pcp
