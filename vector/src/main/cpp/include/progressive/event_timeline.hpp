#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "progressive/event_display.hpp"

namespace progressive {

// ---- Timeline Gap Detection ----

struct TimelineGap {
    int64_t gapStartMs = 0;
    int64_t gapEndMs = 0;
    int64_t gapDurationMs = 0;
    int missingEventsEstimate = 0;
    std::string prevEventId;
    std::string nextEventId;
};

struct TimelineStats {
    std::string roomId;
    int totalEvents = 0;
    int totalGaps = 0;
    int64_t coverageStartMs = 0;
    int64_t coverageEndMs = 0;
    double coveragePercent = 100.0;
    std::vector<TimelineGap> gaps;
    bool hasMoreBackward = true;
    bool hasMoreForward = false;
};

// Detect gaps in the event timeline based on timestamps.
// Events should be sorted chronologically.
TimelineStats analyzeTimeline(const std::string& roomId,
    const std::vector<std::string>& eventIds,
    const std::vector<int64_t>& timestamps,
    bool hasMoreBackward, bool hasMoreForward,
    int64_t maxGapMs = 3600000
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
    int unreadCount = 0;
    int64_t positionMs = 0;
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

// ==== InsertMode enum ====
//
// Original Kotlin: PaginationDirection (FORWARDS="f", BACKWARDS="b")
// Determines how new events are inserted into an existing timeline.
// APPEND: add to end (newer events, forward pagination)
// PREPEND: add to beginning (older events, backward pagination)
// REPLACE: replace all events (initial load or restart)

enum class InsertMode {
    APPEND = 0,
    PREPEND = 1,
    REPLACE = 2
};

const char* insertModeToString(InsertMode m);
InsertMode insertModeFromString(const std::string& s);

// ==== TimelineSnapshot — full timeline state ====
//
// Original Kotlin: Timeline.snapshot (List<TimelineEvent>)
// A point-in-time capture of the timeline for UI rendering.

struct TimelineSnapshot {
    std::vector<EventDisplayInfo> events;   // display-ready events, chronological
    bool hasMoreForward = false;            // can load newer events
    bool hasMoreBackward = true;            // can load older events
    int totalEventCount = 0;               // total events known
    std::string oldestEventId;             // first event ID
    std::string newestEventId;             // last event ID
    std::string readMarkerEventId;         // last read event
    std::string fullyReadEventId;          // room fully read marker
    int64_t oldestTimestamp = 0;
    int64_t newestTimestamp = 0;
};

// ==== TimelineDisplayItem — single display item in timeline ====
//
// Original Kotlin: TimelineChunk builtItems() → displayList
// Represents one cell in the timeline RecyclerView.

struct TimelineDisplayItem {
    EventDisplayInfo event;                    // fully formatted event data
    EventDisplayInfo displayInfo;              // alias for convenience (same as event)
    bool isGrouped = false;                    // merges with previous event cell
    int groupPosition = 0;                     // 0 = first, 1 = middle, 2 = last, 3 = single
    bool showSender = true;                    // show sender name/avatar
    bool showTimestamp = true;                 // show standalone timestamp
    bool isReadMarker = false;                 // "new messages" divider
    bool isNewDay = false;                     // date separator before this item
    std::string dateSeparator;                 // date string (e.g. "Today")
    int index = 0;                             // position in full display list
};

// ==== TimelineDateSeparator ====
//
// Original Kotlin: DateSeparatorItem / HeaderItem
// A date header inserted between events on different days.

struct TimelineDateSeparator {
    std::string dateString;                    // "Today", "Monday", "12 Jan 2023"
    int position = 0;                          // index in display list where separator goes
};

// ==== TimelineSection ====
//
// Original Kotlin: TimelineSection (UI grouping)
// Groups events by recency: new messages section, today, yesterday, older.

struct TimelineSection {
    std::string title;                         // "New Messages", "Today", "Yesterday", "Older"
    std::vector<EventDisplayInfo> events;      // events in this section
    bool isNewMessages = false;                // true for unread section
};

// ==== New Functions ====

// ==== buildTimelineSnapshot — build display-ready timeline from events ====
//
// Original Kotlin: DefaultTimeline.strategy.buildSnapshot()
// Takes raw event data and produces a TimelineSnapshot for UI rendering.
TimelineSnapshot buildTimelineSnapshot(
    const std::vector<std::string>& eventJsons,
    const std::string& myUserId,
    bool hasMoreForward = false,
    bool hasMoreBackward = true,
    const std::string& readMarkerEventId = "",
    const std::string& fullyReadEventId = "");

// ==== computeTimelineDisplayItems — convert raw events to display items with grouping ====
//
// Original Kotlin: TimelineChunk.builtItems() → displayItems
// Applies grouping, sender display, timestamp display, read marker.
std::vector<TimelineDisplayItem> computeTimelineDisplayItems(
    const std::vector<EventDisplayInfo>& events,
    const std::string& readMarkerEventId = "");

// ==== insertDateSeparators — add date separators between days ====
//
// Original Kotlin: DateSeparatorProcessor.insertDateHeaders(items)
// Scans display items and inserts date header items where dates change.
std::vector<TimelineDisplayItem> insertDateSeparators(
    const std::vector<TimelineDisplayItem>& items);

// ==== formatTimelineDate — date headings ====
//
// Original Kotlin: DateFormatter.formatForTimelineHeader(ts)
// Returns: "Today", "Yesterday", "Monday", "12 Jan 2023"
std::string formatTimelineDate(int64_t epochMs);

// ==== getTimelineSections — group into sections ====
//
// Original Kotlin: TimelineSectionGrouper.group(items)
// Groups events into: new messages (after read marker), today, yesterday, older.
std::vector<TimelineSection> getTimelineSections(
    const std::vector<TimelineDisplayItem>& items);

// ==== isReadMarkerPosition — compute where read marker goes ====
//
// Original Kotlin: ReadMarkersTracker.getReadMarkerPosition()
// Returns the index where the "new messages" divider should be inserted,
// or -1 if no read marker is needed.
int isReadMarkerPosition(
    const std::vector<std::string>& eventIds,
    const std::string& readMarkerEventId);

// ==== computeInsertMode — determine insert mode from pagination direction ====
//
// Original Kotlin: PaginationDirection enum mapping
// direction="b" (backwards) → PREPEND
// direction="f" (forwards) → APPEND
// initial load / eventId lookup → REPLACE
InsertMode computeInsertMode(const std::string& direction,
    bool isInitialLoad = false);

// ==== JSON Serialization ====

std::string timelineSnapshotToJson(const TimelineSnapshot& ts);
TimelineSnapshot timelineSnapshotFromJson(const std::string& json);
std::string timelineDisplayItemToJson(const TimelineDisplayItem& item);
TimelineDisplayItem timelineDisplayItemFromJson(const std::string& json);
std::string timelineDateSeparatorToJson(const TimelineDateSeparator& ds);
TimelineDateSeparator timelineDateSeparatorFromJson(const std::string& json);
std::string timelineSectionToJson(const TimelineSection& ts);
TimelineSection timelineSectionFromJson(const std::string& json);
std::string insertModeToJson(InsertMode m);
InsertMode insertModeFromJson(const std::string& json);

} // namespace progressive
