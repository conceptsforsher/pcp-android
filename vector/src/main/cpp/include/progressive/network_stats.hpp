#ifndef PROGRESSIVE_NETWORK_STATS_HPP
#define PROGRESSIVE_NETWORK_STATS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct RequestRecord {
    std::string url;
    std::string method;
    int64_t startTimeMs = 0;    // epoch ms
    int64_t endTimeMs = 0;      // epoch ms
    int statusCode = 0;
    int64_t bytesSent = 0;
    int64_t bytesReceived = 0;
    bool success = false;       // 2xx status
    std::string errorMessage;
};

struct NetworkStats {
    int totalRequests = 0;
    int failedRequests = 0;
    int64_t totalBytesSent = 0;
    int64_t totalBytesReceived = 0;
    double avgLatencyMs = 0.0;    // average request time
    double minLatencyMs = 0.0;
    double maxLatencyMs = 0.0;
    double packetLossRate = 0.0;  // 0.0 to 1.0
    double requestsPerMinute = 0.0;
};

class NetworkStatsCollector {
public:
    // Record the start of a request. Returns a request ID.
    int startRequest(const std::string& url, const std::string& method);

    // Record the completion of a request by ID.
    void endRequest(int requestId, int statusCode, int64_t bytesSent, int64_t bytesReceived,
                    const std::string& errorMessage = "");

    // Compute aggregate stats from all records.
    NetworkStats computeStats() const;

    // Get all records for detailed display.
    const std::vector<RequestRecord>& getRecords() const { return records_; }

    // Clear all records.
    void clear();

    // Format stats as JSON for Kotlin consumption.
    std::string statsToJson() const;

    // Format stats as human-readable text.
    std::string statsToText() const;

    // Format a single record as JSON.
    static std::string recordToJson(const RequestRecord& r);

private:
    std::vector<RequestRecord> records_;
    int nextId_ = 1;
};

// Compute packet loss rate: failed / total
double computePacketLossRate(int total, int failed);

// Compute requests per minute from elapsed time
double computeRequestRate(int totalRequests, int64_t elapsedMs);

} // namespace progressive

#endif // PROGRESSIVE_NETWORK_STATS_HPP
