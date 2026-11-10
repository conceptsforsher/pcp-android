#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ReadTrackerState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* read_tracker_stateToString(ReadTrackerState s);
ReadTrackerState read_tracker_stateFromString(const std::string& s);

struct ReadTrackerConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ReadTrackerRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ReadTrackerResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ReadTrackerStatus {
    ReadTrackerState state = ReadTrackerState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ReadTrackerStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the read tracker module with configuration
bool read_tracker_init(const ReadTrackerConfig& config);

// Handle a read tracker request, returning a response
ReadTrackerResponse read_tracker_handle(const ReadTrackerRequest& req);

// Get the current operational status
ReadTrackerStatus read_tracker_getStatus();

// Reset the module to initial state
void read_tracker_reset();

// Validate input before processing
bool read_tracker_validate(const std::string& input);

// Process input and return result
std::string read_tracker_process(const std::string& input);

// Serialize to JSON format
json read_tracker_toJson(const std::string& input);

// Get runtime statistics
ReadTrackerStats read_tracker_getStats();

// Build config from JSON
ReadTrackerConfig read_tracker_configFromJson(const json& j);

// Serialize config to JSON
json read_tracker_configToJson(const ReadTrackerConfig& config);

} // namespace pcp
