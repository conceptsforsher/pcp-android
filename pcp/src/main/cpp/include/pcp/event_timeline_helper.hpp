#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

enum class TimelineEventPosition { FIRST, MIDDLE, LAST, SINGLE };

struct TimelineEventMeta {
    std::string eventId;
    std::string senderId;
    bool showSender = true;         // show sender name/avatar
    bool showTimestamp = false;     // show time separator
    bool isGrouped = false;         // grouped with previous (same sender within N min)
    TimelineEventPosition position = TimelineEventPosition::MIDDLE;
    int groupIndex = 0;             // position within group
    int groupSize = 0;              // total events in group
};

// Compute timeline event grouping metadata
// Groups consecutive events from same sender within maxGapMs
std::vector<TimelineEventMeta> computeTimelineGroups(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestampsMs,
    int64_t maxGapMs = 300000);    // 5 minutes

// Check if a timestamp gap should show a date separator
bool shouldShowDateSeparator(int64_t prevTs, int64_t currentTs);

// Check if a timestamp gap should show a time separator
bool shouldShowTimeSeparator(int64_t prevTs, int64_t currentTs, int64_t maxGapMs = 900000); // 15 min

// Format timestamp for timeline header
std::string formatTimelineHeader(int64_t timestampMs);
std::string formatTimelineTime(int64_t timestampMs);

// Get relative time string ("just now", "5 min ago", "yesterday", "May 15")
std::string getRelativeTime(int64_t timestampMs);

} // namespace pcp
