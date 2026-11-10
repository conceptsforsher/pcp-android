#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <nlohmann/json.hpp>

namespace pcp {

using json = nlohmann::json;

enum class ThreadSummaryState {
    UNKNOWN = 0,
    INIT = 1,
    PROCESSING = 2,
    DONE = 3,
    ERROR = 4
};

const char* thread_summary_stateToString(ThreadSummaryState s);
ThreadSummaryState thread_summary_stateFromString(const std::string& s);

struct ThreadSummaryConfig {
    std::string serverUrl;           // Matrix homeserver URL
    std::string userId;              // fully qualified MXID
    std::string accessToken;         // Bearer token
    int timeoutMs = 30000;           // request timeout
    int retryCount = 3;              // retry attempts
    bool enableLogging = false;      // debug logging flag
};

struct ThreadSummaryRequest {
    std::string roomId;              // target room (optional)
    std::string eventId;             // target event (optional)
    json payload;                    // Matrix event payload
    std::string transactionId;       // idempotency key
    int64_t timeoutAt = 0;           // absolute timeout timestamp
};

struct ThreadSummaryResponse {
    bool success = false;
    std::string error;               // Matrix error code
    std::string errorMessage;        // human-readable error
    json result;                     // result payload
    int statusCode = 0;              // HTTP status code
    int64_t serverTimestamp = 0;     // origin_server_ts
};

struct ThreadSummaryStatus {
    ThreadSummaryState state = ThreadSummaryState::UNKNOWN;
    bool isInitialized = false;
    std::string lastError;
    int64_t lastActivity = 0;        // epoch ms
    int pendingCount = 0;
};

struct ThreadSummaryStats {
    int totalInvocations = 0;
    int successCount = 0;
    int failureCount = 0;
    int64_t totalProcessingMs = 0;
    int64_t lastInvocationMs = 0;
    double averageMs = 0.0;
    int retryCount = 0;
};

// Initialize the thread summary module with configuration
bool thread_summary_init(const ThreadSummaryConfig& config);

// Handle a thread summary request, returning a response
ThreadSummaryResponse thread_summary_handle(const ThreadSummaryRequest& req);

// Get the current operational status
ThreadSummaryStatus thread_summary_getStatus();

// Reset the module to initial state
void thread_summary_reset();

// Validate input before processing
bool thread_summary_validate(const std::string& input);

// Process input and return result
std::string thread_summary_process(const std::string& input);

// Serialize to JSON format
json thread_summary_toJson(const std::string& input);

// Get runtime statistics
ThreadSummaryStats thread_summary_getStats();

// Build config from JSON
ThreadSummaryConfig thread_summary_configFromJson(const json& j);

// Serialize config to JSON
json thread_summary_configToJson(const ThreadSummaryConfig& config);

} // namespace pcp
