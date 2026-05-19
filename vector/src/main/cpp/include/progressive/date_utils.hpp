#ifndef PROGRESSIVE_DATE_UTILS_HPP
#define PROGRESSIVE_DATE_UTILS_HPP

#include <string>
#include <cstdint>

namespace progressive {

// Format a UTC epoch timestamp for display in the chat timeline.
// Chooses appropriate format based on how recent the event is:
// - Today: "12:34"
// - Yesterday: "Yesterday 12:34"
// - This week: "Monday 12:34"
// - This year: "May 13 12:34"
// - Older: "May 13, 2025 12:34"
// - With seconds: "12:34:56" (if includeSeconds is true)
std::string formatChatTimestamp(int64_t epochMs, bool includeSeconds = false);

// Format just the date part: "May 13, 2025"
std::string formatDate(int64_t epochMs);

// Format just the time part: "12:34"
std::string formatTime(int64_t epochMs, bool includeSeconds = false);

// Format as ISO 8601: "2025-05-13T12:34:56Z"
std::string formatIso8601(int64_t epochMs);

// Format a relative time: "2 minutes ago", "3 hours ago", "2 days ago"
std::string formatRelativeTime(int64_t epochMs, int64_t nowMs = 0);

// Format a duration in ms: "2h 15m", "45s", "3d 2h"
std::string formatDuration(int64_t durationMs);

// Parse an ISO 8601 date string to epoch ms.
int64_t parseIso8601(const std::string& isoDate);

// Get current UTC epoch ms.
int64_t nowMs();

// Check if a timestamp is today.
bool isToday(int64_t epochMs);

// Check if a timestamp is yesterday.
bool isYesterday(int64_t epochMs);

// Check if a timestamp is within the same week.
bool isThisWeek(int64_t epochMs);

// Check if a timestamp is within the same year.
bool isThisYear(int64_t epochMs);

// Get the day name: "Monday", "Tuesday", etc.
std::string getDayName(int64_t epochMs);

// Get the month name: "January", "February", etc.
std::string getMonthName(int month);

} // namespace progressive

#endif // PROGRESSIVE_DATE_UTILS_HPP
