#ifndef PROGRESSIVE_EVENT_TIMELINE_HPP
#define PROGRESSIVE_EVENT_TIMELINE_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Timeline Gap Detection ----

struct TimelineGap {
    int64_t gapStartMs = 0;    // where the gap starts
    int64_t gapEndMs = 0;      // where the gap ends
    int64_t gapDurationMs = 0; // how long the gap is
    int missingEventsEstimate = 0; // estimated events missing
    std::string prevEventId;   // event before gap
    std::string nextEventId;   // event after gap
};

struct TimelineStats {
    std::string roomId;
    int totalEvents = 0;
    int totalGaps = 0;
    int64_t coverageStartMs = 0;
    int64_t coverageEndMs = 0;
    double coveragePercent = 100.0;  // how much of the timeline is loaded
    std::vector<TimelineGap> gaps;    // all detected gaps
    bool hasMoreBackward = true;     // server has older events
    bool hasMoreForward = false;     // server has newer events
};

// Detect gaps in the event timeline based on timestamps.
// Events should be sorted chronologically.
TimelineStats analyzeTimeline(const std::string& roomId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    bool hasMoreBackward, bool hasMoreForward,
    int64_t maxGapMs = 3600000  // gaps > 1 hour are significant
);

// Estimate events in a gap based on average event frequency.
int estimateMissingEvents(int64_t gapDurationMs, double avgIntervalMs);

// Compute average time between events.
double computeAvgIntervalMs(const std::vector<int64_t>& timestamps);

// ---- Event Grouping ----

struct EventGroup {
    std::string groupKey;      // date-based: "2025-05-13"
    std::string label;         // "Today", "Yesterday", "May 13, 2025"
    int eventCount = 0;
    int64_t startMs = 0;
    int64_t endMs = 0;
};

// Group events by date for timeline separators.
std::vector<EventGroup> groupEventsByDate(const std::vector<int64_t>& timestamps);

// Format group label for timeline display.
std::string formatGroupLabel(const EventGroup& group);
std::string formatGroupLabel(int64_t timestampMs);

// ---- Read Marker Logic ----

struct ReadMarker {
    std::string eventId;
    int unreadCount = 0;       // events after this marker
    int64_t positionMs = 0;    // timestamp of the marker
    bool hasUnreadMentions = false;
};

// Compute read marker position from event data.
ReadMarker computeReadMarker(
    const std::string& lastReadEventId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    const std::vector<bool>& isMention,
    const std::string& myUserId
);

// Format timeline stats as JSON.
std::string timelineStatsToJson(const TimelineStats& stats);

} // namespace progressive

#endif // PROGRESSIVE_EVENT_TIMELINE_HPP
