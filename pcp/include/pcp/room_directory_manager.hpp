#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class DirectoryManagerAction {
    NONE = 0,
    VALIDATE = 1,
    EXECUTE = 2,
    COMMIT = 3,
    ROLLBACK = 4
};

const char* room_directory_manager_stateToString(DirectoryManagerAction s);
DirectoryManagerAction room_directory_manager_stateFromString(const std::string& s);

struct RoomDirectoryManagerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct RoomDirectoryManagerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct RoomDirectoryManagerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct RoomDirectoryManagerStatus {
    DirectoryManagerAction state = DirectoryManagerAction::NONE;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct RoomDirectoryManagerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the room directory manager module with configuration
bool room_directory_manager_init(const RoomDirectoryManagerConfig& config);

// Handle a room directory manager request, returning a response
RoomDirectoryManagerResponse room_directory_manager_handle(const RoomDirectoryManagerRequest& req);

// Get the current operational status
RoomDirectoryManagerStatus room_directory_manager_getStatus();

// Reset the module to initial state
void room_directory_manager_reset();

// Validate input before processing
bool room_directory_manager_validate(const std::string& input);

// Process input and return result
std::string room_directory_manager_process(const std::string& input);

// Serialize to JSON format
json room_directory_manager_toJson(const std::string& input);

// Get runtime statistics
RoomDirectoryManagerStats room_directory_manager_getStats();

// Build config from JSON
RoomDirectoryManagerConfig room_directory_manager_configFromJson(const json& j);

// Serialize config to JSON
json room_directory_manager_configToJson(const RoomDirectoryManagerConfig& config);

} // namespace pcp
