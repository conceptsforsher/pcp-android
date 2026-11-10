#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class NoticeFormatterAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_notice_formatter_stateToString(NoticeFormatterAction s);
NoticeFormatterAction room_notice_formatter_stateFromString(const std::string& s);

struct RoomNoticeFormatterConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomNoticeFormatterRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomNoticeFormatterResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomNoticeFormatterStatus {
    NoticeFormatterAction state = NoticeFormatterAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomNoticeFormatterStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room notice formatter module with configuration
bool room_notice_formatter_init(const RoomNoticeFormatterConfig& config);

// Handle a room notice formatter request, returning a response
RoomNoticeFormatterResponse room_notice_formatter_handle(const RoomNoticeFormatterRequest& req);

// Get the current operational status
RoomNoticeFormatterStatus room_notice_formatter_getStatus();

// Reset the module to initial state
void room_notice_formatter_reset();

// Validate input before processing
bool room_notice_formatter_validate(const std::string& input);

// Process input and return result
std::string room_notice_formatter_process(const std::string& input);

// Serialize to JSON format
json room_notice_formatter_toJson(const std::string& input);

// Get runtime statistics
RoomNoticeFormatterStats room_notice_formatter_getStats();

// Build config from JSON
RoomNoticeFormatterConfig room_notice_formatter_configFromJson(const json& j);

// Serialize config to JSON
json room_notice_formatter_configToJson(const RoomNoticeFormatterConfig& config);

} // namespace pcp
