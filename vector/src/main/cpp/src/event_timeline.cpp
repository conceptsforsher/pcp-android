#include "progressive/event_timeline.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <unordered_set>

namespace progressive {

// ---- Timeline Gap Detection ----

double computeAvgIntervalMs(const std::vector<int64_t>& timestamps) {
    if (timestamps.size() < 2) return 0.0;
    int64_t totalInterval = 0;
    for (size_t i = 1; i < timestamps.size(); ++i) {
        totalInterval += timestamps[i] - timestamps[i - 1];
    }
    return static_cast<double>(totalInterval) / (timestamps.size() - 1);
}

int estimateMissingEvents(int64_t gapDurationMs, double avgIntervalMs) {
    if (avgIntervalMs <= 0) return 0;
    return static_cast<int>(gapDurationMs / avgIntervalMs);
}

TimelineStats analyzeTimeline(const std::string& roomId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    bool hasMoreBackward, bool hasMoreForward,
    int64_t maxGapMs
) {
    TimelineStats stats;
    stats.roomId = roomId;
    stats.totalEvents = static_cast<int>(eventIds.size());
    stats.hasMoreBackward = hasMoreBackward;
    stats.hasMoreForward = hasMoreForward;

    if (eventIds.size() < 2) return stats;

    stats.coverageStartMs = timestamps.front();
    stats.coverageEndMs = timestamps.back();

    double avgInterval = computeAvgIntervalMs(timestamps);

    // Detect gaps
    for (size_t i = 1; i < timestamps.size(); ++i) {
        int64_t gapDuration = timestamps[i] - timestamps[i - 1];
        if (gapDuration > maxGapMs) {
            TimelineGap gap;
            gap.gapStartMs = timestamps[i - 1];
            gap.gapEndMs = timestamps[i];
            gap.gapDurationMs = gapDuration;
            gap.missingEventsEstimate = estimateMissingEvents(gapDuration, avgInterval);
            gap.prevEventId = i > 0 ? eventIds[i - 1] : "";
            gap.nextEventId = eventIds[i];
            stats.gaps.push_back(gap);
        }
    }

    stats.totalGaps = static_cast<int>(stats.gaps.size());

    // Coverage
    if (stats.coverageEndMs > stats.coverageStartMs) {
        int64_t totalMs = stats.coverageEndMs - stats.coverageStartMs;
        int64_t gapMs = 0;
        for (const auto& g : stats.gaps) gapMs += g.gapDurationMs;
        stats.coveragePercent = 100.0 * (1.0 - static_cast<double>(gapMs) / totalMs);
    }

    return stats;
}

// ---- Event Grouping ----

std::vector<EventGroup> groupEventsByDate(const std::vector<int64_t>& timestamps) {
    std::vector<EventGroup> groups;

    for (int64_t ts : timestamps) {
        if (ts <= 0) continue;
        time_t t = ts / 1000;
        struct tm result;
        gmtime_r(&t, &result);

        char keyBuf[16];
        strftime(keyBuf, sizeof(keyBuf), "%Y-%m-%d", &result);
        std::string key(keyBuf);

        if (groups.empty() || groups.back().groupKey != key) {
            EventGroup group;
            group.groupKey = key;
            group.startMs = ts;
            group.label = formatGroupLabel(ts);
            groups.push_back(group);
        }
        auto& last = groups.back();
        last.eventCount++;
        last.endMs = ts;
    }

    return groups;
}

std::string formatGroupLabel(int64_t timestampMs) {
    if (timestampMs <= 0) return "";
    time_t now = time(nullptr);
    time_t ts = timestampMs / 1000;
    struct tm nowTm, tsTm;
    gmtime_r(&now, &nowTm);
    gmtime_r(&ts, &tsTm);

    if (nowTm.tm_year == tsTm.tm_year && nowTm.tm_mon == tsTm.tm_mon && nowTm.tm_mday == tsTm.tm_mday) {
        return "Today";
    }

    time_t yesterday = now - 86400;
    struct tm yestTm;
    gmtime_r(&yesterday, &yestTm);
    if (yestTm.tm_year == tsTm.tm_year && yestTm.tm_mon == tsTm.tm_mon && yestTm.tm_mday == tsTm.tm_mday) {
        return "Yesterday";
    }

    char buf[32];
    if (nowTm.tm_year == tsTm.tm_year) {
        strftime(buf, sizeof(buf), "%B %d", &tsTm);
    } else {
        strftime(buf, sizeof(buf), "%B %d, %Y", &tsTm);
    }
    return std::string(buf);
}

std::string formatGroupLabel(const EventGroup& group) {
    return group.label;
}

// ---- Read Marker ----

ReadMarker computeReadMarker(
    const std::string& lastReadEventId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    const std::vector<bool>& isMention,
    const std::string& myUserId
) {
    ReadMarker marker;
    marker.eventId = lastReadEventId;

    // Find position of last read event
    size_t readPos = 0;
    for (size_t i = 0; i < eventIds.size(); ++i) {
        if (eventIds[i] == lastReadEventId) {
            readPos = i;
            if (i < timestamps.size()) marker.positionMs = timestamps[i];
            break;
        }
    }

    // Count unread events after this position
    for (size_t i = readPos + 1; i < eventIds.size(); ++i) {
        marker.unreadCount++;
        if (i < isMention.size() && isMention[i]) {
            marker.hasUnreadMentions = true;
        }
    }

    return marker;
}

std::string timelineStatsToJson(const TimelineStats& stats) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";
    json << R"("roomId": ")" << esc(stats.roomId) << R"(",)";
    json << R"("totalEvents": )" << stats.totalEvents << ",";
    json << R"("totalGaps": )" << stats.totalGaps << ",";
    json << R"("coveragePercent": )" << stats.coveragePercent << ",";
    json << R"("gaps": [)";
    for (size_t i = 0; i < stats.gaps.size(); ++i) {
        if (i > 0) json << ",";
        const auto& g = stats.gaps[i];
        json << R"({"gapDurationMs": )" << g.gapDurationMs;
        json << R"(,"missingEstimate": )" << g.missingEventsEstimate;
        json << R"(,"prevEventId": ")" << esc(g.prevEventId) << R"(")";
        json << R"(,"nextEventId": ")" << esc(g.nextEventId) << R"(")" << "}";
    }
    json << "]}";
    return json.str();
}

} // namespace progressive
