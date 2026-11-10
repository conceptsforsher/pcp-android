#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class TaskQueueState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* task_queue_stateToString(TaskQueueState s);
TaskQueueState task_queue_stateFromString(const std::string& s);

struct TaskQueueConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct TaskQueueRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct TaskQueueResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct TaskQueueStatus {
    TaskQueueState state = TaskQueueState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct TaskQueueStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the task queue module with configuration
bool task_queue_init(const TaskQueueConfig& config);

// Handle a task queue request, returning a response
TaskQueueResponse task_queue_handle(const TaskQueueRequest& req);

// Get the current operational status
TaskQueueStatus task_queue_getStatus();

// Reset the module to initial state
void task_queue_reset();

// Validate input before processing
bool task_queue_validate(const std::string& input);

// Process input and return result
std::string task_queue_process(const std::string& input);

// Serialize to JSON format
json task_queue_toJson(const std::string& input);

// Get runtime statistics
TaskQueueStats task_queue_getStats();

// Build config from JSON
TaskQueueConfig task_queue_configFromJson(const json& j);

// Serialize config to JSON
json task_queue_configToJson(const TaskQueueConfig& config);

} // namespace pcp
