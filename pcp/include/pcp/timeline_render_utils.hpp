#include <nlohmann/json.hpp>
#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

enum class TimelineItemType { MESSAGE, STATE_EVENT, ROOM_CREATE, MEMBERSHIP,
                                ENCRYPTION, TOMBSTONE, REDACTION, WIDGET, CALL, POLL,
                                VOICE_BROADCAST, STICKER, LOCATION, MERGED_HEADER,
                                DATE_SEPARATOR, TIME_SEPARATOR, READ_MARKER, UNKNOWN };

struct TimelineItemMeta {
    std::string eventId;
    TimelineItemType type = TimelineItemType::UNKNOWN;
    int displayIndex = 0;
    bool isGrouped = false;         // same sender as previous
    bool isGroupStart = false;      // first in group
    bool isGroupEnd = false;        // last in group
    bool showSender = true;
    bool showTimestamp = false;
    bool showDateSeparator = false;
    std::string dateLabel;          // "May 22, 2026"
    std::string timeLabel;          // "12:34"
};

// Classify event for timeline rendering
TimelineItemType classifyTimelineItem(const std::string& eventJson);

// Compute timeline item metadata (grouping, separators)
std::vector<TimelineItemMeta> computeTimelineMeta(
    const std::vector<std::string>& eventIds,
    const std::vector<std::string>& eventJsons,
    const std::vector<std::string>& senderIds,
    const std::vector<int64_t>& timestamps);

// Format date separator ("── May 22, 2026 ──")
std::string formatDateSeparator(const std::string& date);

// Format "new messages" divider
std::string formatNewMessagesDivider(int count);

// Check if event should be grouped with previous
bool shouldGroupEvents(const std::string& sender1, const std::string& sender2,
                        int64_t ts1, int64_t ts2, int64_t maxGapMs = 300000);

} // namespace pcp
