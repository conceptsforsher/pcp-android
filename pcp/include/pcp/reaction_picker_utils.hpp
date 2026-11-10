#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReactionPickerUtilsState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* reaction_picker_utils_stateToString(ReactionPickerUtilsState s);
ReactionPickerUtilsState reaction_picker_utils_stateFromString(const std::string& s);

struct ReactionPickerUtilsConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReactionPickerUtilsRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReactionPickerUtilsResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReactionPickerUtilsStatus {
    ReactionPickerUtilsState state = ReactionPickerUtilsState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReactionPickerUtilsStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the reaction picker utils module with configuration
bool reaction_picker_utils_init(const ReactionPickerUtilsConfig& config);

// Handle a reaction picker utils request, returning a response
ReactionPickerUtilsResponse reaction_picker_utils_handle(const ReactionPickerUtilsRequest& req);

// Get the current operational status
ReactionPickerUtilsStatus reaction_picker_utils_getStatus();

// Reset the module to initial state
void reaction_picker_utils_reset();

// Validate input before processing
bool reaction_picker_utils_validate(const std::string& input);

// Process input and return result
std::string reaction_picker_utils_process(const std::string& input);

// Serialize to JSON format
json reaction_picker_utils_toJson(const std::string& input);

// Get runtime statistics
ReactionPickerUtilsStats reaction_picker_utils_getStats();

// Build config from JSON
ReactionPickerUtilsConfig reaction_picker_utils_configFromJson(const json& j);

// Serialize config to JSON
json reaction_picker_utils_configToJson(const ReactionPickerUtilsConfig& config);

} // namespace pcp
