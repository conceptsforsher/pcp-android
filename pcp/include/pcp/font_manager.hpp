#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class FontManagerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* font_manager_stateToString(FontManagerState s);
FontManagerState font_manager_stateFromString(const std::string& s);

struct FontManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct FontManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct FontManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct FontManagerStatus {
    FontManagerState state = FontManagerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct FontManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the font manager module with configuration
bool font_manager_init(const FontManagerConfig& config);

// Handle a font manager request, returning a response
FontManagerResponse font_manager_handle(const FontManagerRequest& req);

// Get the current operational status
FontManagerStatus font_manager_getStatus();

// Reset the module to initial state
void font_manager_reset();

// Validate input before processing
bool font_manager_validate(const std::string& input);

// Process input and return result
std::string font_manager_process(const std::string& input);

// Serialize to JSON format
json font_manager_toJson(const std::string& input);

// Get runtime statistics
FontManagerStats font_manager_getStats();

// Build config from JSON
FontManagerConfig font_manager_configFromJson(const json& j);

// Serialize config to JSON
json font_manager_configToJson(const FontManagerConfig& config);

} // namespace pcp
