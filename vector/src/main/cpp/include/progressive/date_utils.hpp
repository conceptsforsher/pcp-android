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

// Original Kotlin: DateFormat enum for formatting style selection
enum class DateFormat {
    ISO_8601,
    RFC_2822,
    SHORT_DATE,         // 05/13/25
    LONG_DATE,          // May 13, 2025
    SHORT_TIME,         // 12:34
    LONG_TIME,          // 12:34:56
    SHORT_DATETIME,     // 05/13/25 12:34
    LONG_DATETIME,      // May 13, 2025 12:34
    RELATIVE,           // "just now", "5m ago", "yesterday"
    RELATIVE_SHORT,     // "5m", "2h", "1d"
    DURATION            // "1:23:45" or "2h 30m"
};

// Original Kotlin: formatTimestamp — general purpose timestamp formatting
std::string formatTimestamp(int64_t epochMs, DateFormat fmt, int64_t nowMsRef = 0);

// Original Kotlin: formatRelativeShort — compact relative: "5m", "2h", "1d"
std::string formatRelativeShort(int64_t epochMs, int64_t nowMsRef = 0);

// Original Kotlin: parseRfc2822 — parse RFC 2822 date string to epoch ms
int64_t parseRfc2822(const std::string& rfcDate);

// Original Kotlin: DateRange — interval between two timestamps
struct DateRange {
    int64_t start = 0;
    int64_t end = 0;
};

// Original Kotlin: isDateInRange — check if timestamp falls within range
bool isDateInRange(int64_t epochMs, const DateRange& range);

// Original Kotlin: daysBetween — number of calendar days between two timestamps
int daysBetween(int64_t startMs, int64_t endMs);

// Original Kotlin: getStartOfDay — epoch ms of midnight UTC for given timestamp
int64_t getStartOfDay(int64_t epochMs);

// Original Kotlin: getEndOfDay — epoch ms of 23:59:59.999 UTC for given timestamp
int64_t getEndOfDay(int64_t epochMs);

// Original Kotlin: getStartOfWeek — epoch ms of previous Monday 00:00 UTC
int64_t getStartOfWeek(int64_t epochMs);

// Original Kotlin: getStartOfMonth — epoch ms of 1st of month at 00:00 UTC
int64_t getStartOfMonth(int64_t epochMs);

// Original Kotlin: isThisMonth — check if timestamp is in current month
bool isThisMonth(int64_t epochMs);

// Original Kotlin: CalendarMonth — structured calendar month data
struct CalendarMonth {
    int year = 0;
    int month = 0;      // 1-12
    std::string name;   // "January", etc.
    int days = 0;       // days in this month
    int firstDayOffset = 0; // 0=Sunday, offset for first day of month
};

// Original Kotlin: buildCalendarMonth — create CalendarMonth from year/month
CalendarMonth buildCalendarMonth(int year, int month);

// Original Kotlin: getShortDayName — abbreviated day: "Mon"
std::string getShortDayName(int64_t epochMs);

// Original Kotlin: getShortMonthName — abbreviated month: "Jan"
std::string getShortMonthName(int month);

// Original Kotlin: addDays — offset timestamp by N days
int64_t addDays(int64_t epochMs, int days);

// Original Kotlin: addMonths — offset timestamp by N months
int64_t addMonths(int64_t epochMs, int months);

// Original Kotlin: addYears — offset timestamp by N years
int64_t addYears(int64_t epochMs, int years);

// Original Kotlin: getYear — extract year component
int getYear(int64_t epochMs);

// Original Kotlin: getMonth — extract month component (1-12)
int getMonth(int64_t epochMs);

// Original Kotlin: getDayOfMonth — extract day component (1-31)
int getDayOfMonth(int64_t epochMs);

// Original Kotlin: getDayOfWeek — extract weekday (0=Sunday, 6=Saturday)
int getDayOfWeek(int64_t epochMs);

// Original Kotlin: getHour — extract hour component (0-23)
int getHour(int64_t epochMs);

// Original Kotlin: getMinute — extract minute component (0-59)
int getMinute(int64_t epochMs);

// Original Kotlin: isSameDay — check if two timestamps fall on same calendar day
bool isSameDay(int64_t epochMsA, int64_t epochMsB);

// Original Kotlin: formatDurationHMS — duration as H:MM:SS or M:SS
std::string formatDurationHMS(int64_t durationMs);

// Original Kotlin: getWeekOfYear — ISO week number (1-53)
int getWeekOfYear(int64_t epochMs);

// Original Kotlin: getQuarter — quarter of year (1-4)
int getQuarter(int month);

// Original Kotlin: getAge — age in years from birth timestamp
int getAge(int64_t birthEpochMs, int64_t nowMsRef = 0);

// Original Kotlin: getDayOfYear — day of year (1-366)
int getDayOfYear(int64_t epochMs);

// Original Kotlin: isLeapYear — check if year is leap year
bool isLeapYear(int year);

// Original Kotlin: daysInMonth — days in given month/year
int daysInMonth(int year, int month);

// Original Kotlin: getStartOfYear — Jan 1 00:00 UTC
int64_t getStartOfYear(int year);

// Original Kotlin: getEndOfMonth — last millisecond of month
int64_t getEndOfMonth(int64_t epochMs);

// Original Kotlin: diffDays — signed days between two timestamps
int diffDays(int64_t fromMs, int64_t toMs);

// Original Kotlin: diffHours — signed hours between two timestamps
int diffHours(int64_t fromMs, int64_t toMs);

// Original Kotlin: diffMinutes — signed minutes between two timestamps
int diffMinutes(int64_t fromMs, int64_t toMs);

// Original Kotlin: roundToNearestMinute — round to nearest minute
int64_t roundToNearestMinute(int64_t epochMs);

// Original Kotlin: roundToNearestHour — round to nearest hour
int64_t roundToNearestHour(int64_t epochMs);

// Original Kotlin: formatMonthYear — "May 2025"
std::string formatMonthYear(int64_t epochMs);

// Original Kotlin: getMillisUntil — ms until target (negative if past)
int64_t getMillisUntil(int64_t targetMs, int64_t nowMsRef = 0);

// Original Kotlin: getTimeComponents — extract hour/minute/second
void getTimeComponents(int64_t epochMs, int& hour, int& minute, int& second);

// Original Kotlin: getDateComponents — extract year/month/day
void getDateComponents(int64_t epochMs, int& year, int& month, int& day);

// Original Kotlin: formatRelativeTimeVerbose — more descriptive relative time
std::string formatRelativeTimeVerbose(int64_t epochMs, int64_t nowMsRef = 0);

} // namespace progressive

#endif // PROGRESSIVE_DATE_UTILS_HPP
