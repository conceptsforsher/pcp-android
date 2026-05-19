#include "progressive/network_stats.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cmath>

namespace progressive {

int NetworkStatsCollector::startRequest(const std::string& url, const std::string& method) {
    RequestRecord rec;
    rec.url = url;
    rec.method = method;
    rec.startTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    int id = nextId_++;
    records_.push_back(rec);
    return id;
}

void NetworkStatsCollector::endRequest(
    int requestId, int statusCode,
    int64_t bytesSent, int64_t bytesReceived,
    const std::string& errorMessage
) {
    // Find the record by position (ID = index + 1)
    int index = requestId - 1;
    if (index < 0 || index >= static_cast<int>(records_.size())) return;

    auto& rec = records_[index];
    rec.endTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    rec.statusCode = statusCode;
    rec.bytesSent = bytesSent;
    rec.bytesReceived = bytesReceived;
    rec.errorMessage = errorMessage;
    rec.success = (statusCode >= 200 && statusCode < 300);
}

NetworkStats NetworkStatsCollector::computeStats() const {
    NetworkStats stats;
    std::vector<double> latencies;

    int64_t firstTime = 0, lastTime = 0;

    for (const auto& rec : records_) {
        stats.totalBytesSent += rec.bytesSent;
        stats.totalBytesReceived += rec.bytesReceived;

        if (rec.endTimeMs > 0) {
            ++stats.totalRequests;
            if (!rec.success) ++stats.failedRequests;

            double latency = static_cast<double>(rec.endTimeMs - rec.startTimeMs);
            latencies.push_back(latency);

            if (firstTime == 0 || rec.startTimeMs < firstTime) firstTime = rec.startTimeMs;
            if (rec.endTimeMs > lastTime) lastTime = rec.endTimeMs;
        }
    }

    if (!latencies.empty()) {
        std::sort(latencies.begin(), latencies.end());
        stats.minLatencyMs = latencies.front();
        stats.maxLatencyMs = latencies.back();

        double sum = 0.0;
        for (auto l : latencies) sum += l;
        stats.avgLatencyMs = sum / latencies.size();
    }

    if (stats.totalRequests > 0) {
        stats.packetLossRate = computePacketLossRate(stats.totalRequests, stats.failedRequests);
    }

    int64_t elapsed = (lastTime > firstTime) ? (lastTime - firstTime) : 3600000; // default 1h
    stats.requestsPerMinute = computeRequestRate(stats.totalRequests, elapsed);

    return stats;
}

void NetworkStatsCollector::clear() {
    records_.clear();
    nextId_ = 1;
}

std::string NetworkStatsCollector::statsToJson() const {
    auto stats = computeStats();
    auto escape = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("totalRequests": )" << stats.totalRequests << ",";
    json << R"("failedRequests": )" << stats.failedRequests << ",";
    json << R"("totalBytesSent": )" << stats.totalBytesSent << ",";
    json << R"("totalBytesReceived": )" << stats.totalBytesReceived << ",";
    json << R"("avgLatencyMs": )" << stats.avgLatencyMs << ",";
    json << R"("minLatencyMs": )" << stats.minLatencyMs << ",";
    json << R"("maxLatencyMs": )" << stats.maxLatencyMs << ",";
    json << R"("packetLossRate": )" << stats.packetLossRate << ",";
    json << R"("requestsPerMinute": )" << stats.requestsPerMinute;
    json << "}";
    return json.str();
}

std::string NetworkStatsCollector::statsToText() const {
    auto stats = computeStats();
    std::ostringstream out;
    out << "Network Statistics\n";
    out << "==================\n";
    out << "Total requests: " << stats.totalRequests << "\n";
    out << "Failed: " << stats.failedRequests << " ("
        << (stats.packetLossRate * 100) << "% loss)\n";
    out << "Avg latency: " << static_cast<int>(stats.avgLatencyMs) << " ms\n";
    out << "Min/Max latency: " << static_cast<int>(stats.minLatencyMs) << " / "
        << static_cast<int>(stats.maxLatencyMs) << " ms\n";
    out << "Sent: " << (stats.totalBytesSent / 1024) << " KB\n";
    out << "Received: " << (stats.totalBytesReceived / 1024) << " KB\n";
    out << "Rate: " << stats.requestsPerMinute << " req/min\n";
    return out.str();
}

std::string NetworkStatsCollector::recordToJson(const RequestRecord& r) {
    auto escape = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            if (c == '"') out += "\\\"";
            else if (c == '\\') out += "\\\\";
            else out += c;
        }
        return out;
    };

    std::ostringstream json;
    json << "{";
    json << R"("url": ")" << escape(r.url) << R"(",)";
    json << R"("method": ")" << r.method << R"(",)";
    json << R"("latencyMs": )" << (r.endTimeMs - r.startTimeMs) << ",";
    json << R"("statusCode": )" << r.statusCode << ",";
    json << R"("success": )" << (r.success ? "true" : "false");
    if (!r.errorMessage.empty()) {
        json << R"(,"error": ")" << escape(r.errorMessage) << R"(")";
    }
    json << "}";
    return json.str();
}

double computePacketLossRate(int total, int failed) {
    if (total == 0) return 0.0;
    return static_cast<double>(failed) / static_cast<double>(total);
}

double computeRequestRate(int totalRequests, int64_t elapsedMs) {
    if (elapsedMs <= 0) return 0.0;
    double minutes = static_cast<double>(elapsedMs) / 60000.0;
    return static_cast<double>(totalRequests) / minutes;
}

} // namespace progressive
