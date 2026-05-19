#include "progressive/sync_analyzer.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>

namespace progressive {

SyncStats analyzeSyncHistory(const std::vector<SyncEvent>& history) {
    SyncStats stats;
    if (history.empty()) return stats;

    stats.totalSyncs = static_cast<int>(history.size());
    int64_t firstTs = 0, lastTs = 0;
    double totalDuration = 0.0;
    int totalEvents = 0, totalRooms = 0;

    for (const auto& e : history) {
        switch (e.type[0]) {
            case 'c': // complete
                stats.successfulSyncs++;
                stats.lastSuccessfulMs = e.timestampMs;
                break;
            case 'e': stats.failedSyncs++; break;
            case 't': stats.timeoutSyncs++; break;
        }

        totalDuration += e.durationMs;
        totalEvents += e.eventsReceived;
        totalRooms += e.roomsUpdated;

        if (firstTs == 0 || e.timestampMs < firstTs) firstTs = e.timestampMs;
        if (e.timestampMs > lastTs) lastTs = e.timestampMs;
    }

    stats.lastSyncMs = lastTs;
    stats.totalEventsReceived = totalEvents;
    stats.totalRoomsUpdated = totalRooms;
    stats.totalUptimeMs = lastTs - firstTs;

    if (stats.totalSyncs > 0) {
        stats.avgDurationMs = totalDuration / stats.totalSyncs;
        stats.avgEventsPerSync = static_cast<double>(totalEvents) / stats.totalSyncs;
        stats.successRate = static_cast<double>(stats.successfulSyncs) / stats.totalSyncs;
    }

    return stats;
}

bool isSyncHealthy(const SyncStats& stats, int64_t maxGapMs) {
    if (stats.successRate < 0.8) return false;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    if (now - stats.lastSuccessfulMs > maxGapMs) return false;
    return true;
}

int suggestSyncTimeout(const SyncStats& stats) {
    if (stats.avgDurationMs <= 0) return 30000; // default 30s

    // Timeout = 3x average duration, clamped to 10s-120s
    int timeout = static_cast<int>(stats.avgDurationMs * 3.0);
    if (timeout < 10000) timeout = 10000;
    if (timeout > 120000) timeout = 120000;
    return timeout;
}

InitSyncProgress updateInitSyncProgress(
    InitSyncProgress current,
    int newRooms, int newEvents, const std::string& currentRoom
) {
    if (current.startedAtMs == 0) {
        current.startedAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    current.processedRooms += newRooms;
    current.processedEvents += newEvents;
    current.currentRoom = currentRoom;

    if (current.totalRooms > 0) {
        current.progressPercent = (current.processedRooms * 100.0) / current.totalRooms;
    }

    if (current.processedRooms >= current.totalRooms && current.totalRooms > 0) {
        current.isComplete = true;
        current.progressPercent = 100.0;
    }

    current.estimatedRemainingMs = estimateRemainingTime(current);
    return current;
}

int64_t estimateRemainingTime(const InitSyncProgress& progress) {
    if (progress.processedRooms == 0 || progress.totalRooms == 0) return 0;

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    int64_t elapsedMs = now - progress.startedAtMs;
    if (elapsedMs <= 0) return 0;

    double rate = static_cast<double>(progress.processedRooms) / elapsedMs;
    int remaining = progress.totalRooms - progress.processedRooms;
    if (rate <= 0) return 0;

    return static_cast<int64_t>(remaining / rate);
}

std::string syncStatsToJson(const SyncStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("totalSyncs": )" << stats.totalSyncs << ",";
    json << R"("successful": )" << stats.successfulSyncs << ",";
    json << R"("failed": )" << stats.failedSyncs << ",";
    json << R"("successRate": )" << stats.successRate << ",";
    json << R"("avgDurationMs": )" << stats.avgDurationMs << ",";
    json << R"("avgEventsPerSync": )" << stats.avgEventsPerSync << ",";
    json << R"("lastSuccessfulMs": )" << stats.lastSuccessfulMs;
    json << "}";
    return json.str();
}

std::string syncStatsToText(const SyncStats& stats) {
    std::ostringstream out;
    out << "Sync Stats\n";
    out << "==========\n";
    out << "Total: " << stats.totalSyncs << " ("
        << stats.successfulSyncs << " ok, "
        << stats.failedSyncs << " failed, "
        << stats.timeoutSyncs << " timeout)\n";
    out << "Success rate: " << (stats.successRate * 100) << "%\n";
    out << "Avg duration: " << static_cast<int>(stats.avgDurationMs) << "ms\n";
    out << "Avg events/sync: " << static_cast<int>(stats.avgEventsPerSync) << "\n";
    return out.str();
}

std::string initSyncProgressToJson(const InitSyncProgress& progress) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("totalRooms": )" << progress.totalRooms << ",";
    json << R"("processedRooms": )" << progress.processedRooms << ",";
    json << R"("progressPercent": )" << progress.progressPercent << ",";
    json << R"("currentRoom": ")" << esc(progress.currentRoom) << R"(",)";
    json << R"("estimatedRemainingMs": )" << progress.estimatedRemainingMs << ",";
    json << R"("isComplete": )" << (progress.isComplete ? "true" : "false");
    json << "}";
    return json.str();
}

std::string formatProgressBar(double percent, int width) {
    int filled = static_cast<int>(percent / 100.0 * width);
    if (filled < 0) filled = 0;
    if (filled > width) filled = width;

    std::string bar = "[";
    for (int i = 0; i < width; ++i) {
        bar += (i < filled) ? '=' : (i == filled) ? '>' : ' ';
    }
    bar += "] " + std::to_string(static_cast<int>(percent)) + "%";
    return bar;
}

} // namespace progressive
