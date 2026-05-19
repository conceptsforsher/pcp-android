#include "progressive/latency_stats.hpp"
#include <sstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iomanip>

namespace progressive {

void LatencyTracker::record(double latencyMs, const std::string& serverName, const std::string& endpoint, bool success) {
    samples_.push_back({
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count(),
        latencyMs, serverName, endpoint, success
    });
}

LatencyStats LatencyTracker::computeStats() const {
    LatencyStats stats;
    if (samples_.empty()) return stats;

    stats.totalSamples = static_cast<int>(samples_.size());
    stats.windowStartMs = samples_.front().timestampMs;
    stats.windowEndMs = samples_.back().timestampMs;

    // Collect successful latencies
    std::vector<double> lats;
    lats.reserve(samples_.size());
    for (const auto& s : samples_) {
        if (s.success) {
            lats.push_back(s.latencyMs);
        } else {
            stats.failedSamples++;
        }
    }

    if (lats.empty()) return stats;

    // Sort for percentiles
    std::sort(lats.begin(), lats.end());

    int n = static_cast<int>(lats.size());

    // Basic stats
    stats.minMs = lats.front();
    stats.maxMs = lats.back();

    double sum = 0.0;
    for (double l : lats) sum += l;
    stats.avgMs = sum / n;

    stats.medianMs = computePercentile(lats, 0.50);
    stats.p95Ms = computePercentile(lats, 0.95);
    stats.p99Ms = computePercentile(lats, 0.99);

    // Standard deviation
    double sqSum = 0.0;
    for (double l : lats) sqSum += (l - stats.avgMs) * (l - stats.avgMs);
    stats.stdDevMs = std::sqrt(sqSum / n);

    // Jitter: average variation between consecutive samples
    if (n >= 2) {
        double jitterSum = 0.0;
        for (int i = 1; i < n; ++i) {
            jitterSum += std::abs(lats[i] - lats[i - 1]);
        }
        stats.jitterMs = jitterSum / (n - 1);
    }

    // Packet loss
    stats.packetLossRate = stats.totalSamples > 0
        ? static_cast<double>(stats.failedSamples) / stats.totalSamples
        : 0.0;

    return stats;
}

LatencyStats LatencyTracker::computeServerStats(const std::string& serverName) const {
    // Filter samples by server, then compute
    std::vector<LatencySample> filtered;
    for (const auto& s : samples_) {
        if (s.serverName == serverName) filtered.push_back(s);
    }
    LatencyTracker temp;
    temp.samples_ = filtered;
    return temp.computeStats();
}

void LatencyTracker::prune(int maxAgeSeconds) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int64_t cutoff = now - (static_cast<int64_t>(maxAgeSeconds) * 1000LL);

    samples_.erase(std::remove_if(samples_.begin(), samples_.end(),
        [cutoff](const LatencySample& s) { return s.timestampMs < cutoff; }
    ), samples_.end());
}

void LatencyTracker::clear() {
    samples_.clear();
}

double LatencyTracker::computePercentile(std::vector<double> sorted, double percentile) {
    if (sorted.empty()) return 0.0;
    int n = static_cast<int>(sorted.size());
    double idx = percentile * (n - 1);
    int lo = static_cast<int>(idx);
    int hi = std::min(lo + 1, n - 1);
    double frac = idx - lo;
    return sorted[lo] * (1.0 - frac) + sorted[hi] * frac;
}

std::string LatencyTracker::formatLatency(double ms) {
    std::ostringstream out;
    if (ms < 1.0) {
        out << static_cast<int>(ms * 1000) << "μs";
    } else if (ms < 1000.0) {
        out << static_cast<int>(ms) << "ms";
    } else {
        out << std::fixed << std::setprecision(1) << (ms / 1000.0) << "s";
    }
    return out.str();
}

std::string LatencyTracker::statsToText(const LatencyStats& stats) {
    std::ostringstream out;
    out << "Latency Stats (" << stats.totalSamples << " samples)\n";
    out << "  Avg:   " << formatLatency(stats.avgMs) << "\n";
    out << "  Min:   " << formatLatency(stats.minMs) << "\n";
    out << "  Max:   " << formatLatency(stats.maxMs) << "\n";
    out << "  Med:   " << formatLatency(stats.medianMs) << "\n";
    out << "  P95:   " << formatLatency(stats.p95Ms) << "\n";
    out << "  P99:   " << formatLatency(stats.p99Ms) << "\n";
    out << "  σ:     " << formatLatency(stats.stdDevMs) << "\n";
    out << "  Jitter:" << formatLatency(stats.jitterMs) << "\n";
    out << "  Loss:  " << (stats.packetLossRate * 100.0) << "%\n";
    return out.str();
}

std::string LatencyTracker::statsToJson(const LatencyStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("avgMs": )" << stats.avgMs << ",";
    json << R"("minMs": )" << stats.minMs << ",";
    json << R"("maxMs": )" << stats.maxMs << ",";
    json << R"("medianMs": )" << stats.medianMs << ",";
    json << R"("p95Ms": )" << stats.p95Ms << ",";
    json << R"("p99Ms": )" << stats.p99Ms << ",";
    json << R"("stdDevMs": )" << stats.stdDevMs << ",";
    json << R"("jitterMs": )" << stats.jitterMs << ",";
    json << R"("packetLossRate": )" << stats.packetLossRate << ",";
    json << R"("totalSamples": )" << stats.totalSamples << ",";
    json << R"("failedSamples": )" << stats.failedSamples;
    json << "}";
    return json.str();
}

} // namespace progressive
