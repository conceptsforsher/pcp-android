#pragma once
#include <string>
#include <cstdint>

namespace pcp {

// Format event timestamp for display
std::string formatEventTime(int64_t timestampMs);

// Format timestamp for timeline: "12:34", "Yesterday 12:34", "May 15, 12:34"
std::string formatTimelineTimestamp(int64_t timestampMs);

// Format timestamp for message detail: "May 15, 2024 12:34:56"
std::string formatDetailTimestamp(int64_t timestampMs);

// Get relative time string: "just now", "5m ago", "2h ago", "3d ago", "May 15"
std::string formatRelativeTimestamp(int64_t timestampMs);

// Check if two timestamps are on the same day
bool isSameDay(int64_t ts1, int64_t ts2);

// Check if timestamp is today
bool isToday(int64_t timestampMs);

// Check if timestamp is yesterday
bool isYesterday(int64_t timestampMs);

// Check if timestamp is within last N minutes
bool isRecent(int64_t timestampMs, int minutes = 15);

// Get current time in milliseconds
int64_t getCurrentTimeMs();

} // namespace pcp
