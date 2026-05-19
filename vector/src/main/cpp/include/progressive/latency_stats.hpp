#ifndef PROGRESSIVE_LATENCY_STATS_HPP
#define PROGRESSIVE_LATENCY_STATS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct LatencySample {
    int64_t timestampMs = 0;   // when measured
    double latencyMs = 0.0;    // round-trip time
    std::string serverName;    // which homeserver
    std::string endpoint;      // which API endpoint
    bool success = true;       // 2xx response
};

struct LatencyStats {
    double avgMs = 0.0;
    double minMs = 0.0;
    double maxMs = 0.0;
    double medianMs = 0.0;
    double p95Ms = 0.0;       // 95th percentile
    double p99Ms = 0.0;       // 99th percentile
    double stdDevMs = 0.0;    // standard deviation
    double jitterMs = 0.0;    // avg variation between consecutive samples
    double packetLossRate = 0.0;
    int totalSamples = 0;
    int failedSamples = 0;
    int64_t windowStartMs = 0;
    int64_t windowEndMs = 0;
};

class LatencyTracker {
public:
    // Record a latency measurement.
    void record(double latencyMs, const std::string& serverName, const std::string& endpoint, bool success = true);

    // Compute aggregated stats from recent samples.
    LatencyStats computeStats() const;

    // Compute stats for a specific server.
    LatencyStats computeServerStats(const std::string& serverName) const;

    // Remove samples older than N seconds.
    void prune(int maxAgeSeconds);

    // Get sample count.
    int sampleCount() const { return static_cast<int>(samples_.size()); }

    void clear();

    // Format stats as human-readable text.
    static std::string statsToText(const LatencyStats& stats);

    // Format stats as JSON.
    static std::string statsToJson(const LatencyStats& stats);

    static std::string formatLatency(double ms);

private:
    std::vector<LatencySample> samples_;
    static double computePercentile(std::vector<double> sorted, double percentile);
};

} // namespace progressive

#endif // PROGRESSIVE_LATENCY_STATS_HPP
