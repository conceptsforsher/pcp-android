#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class EmojiReactionPickerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* emoji_reaction_picker_stateToString(EmojiReactionPickerState s);
EmojiReactionPickerState emoji_reaction_picker_stateFromString(const std::string& s);

struct EmojiReactionPickerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct EmojiReactionPickerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct EmojiReactionPickerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct EmojiReactionPickerStatus {
    EmojiReactionPickerState state = EmojiReactionPickerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct EmojiReactionPickerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the emoji reaction picker module with configuration
bool emoji_reaction_picker_init(const EmojiReactionPickerConfig& config);

// Handle a emoji reaction picker request, returning a response
EmojiReactionPickerResponse emoji_reaction_picker_handle(const EmojiReactionPickerRequest& req);

// Get the current operational status
EmojiReactionPickerStatus emoji_reaction_picker_getStatus();

// Reset the module to initial state
void emoji_reaction_picker_reset();

// Validate input before processing
bool emoji_reaction_picker_validate(const std::string& input);

// Process input and return result
std::string emoji_reaction_picker_process(const std::string& input);

// Serialize to JSON format
json emoji_reaction_picker_toJson(const std::string& input);

// Get runtime statistics
EmojiReactionPickerStats emoji_reaction_picker_getStats();

// Build config from JSON
EmojiReactionPickerConfig emoji_reaction_picker_configFromJson(const json& j);

// Serialize config to JSON
json emoji_reaction_picker_configToJson(const EmojiReactionPickerConfig& config);

} // namespace pcp
