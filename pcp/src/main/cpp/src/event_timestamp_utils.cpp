#include "pcp/event_timestamp_utils.hpp"
#include <chrono>
#include <ctime>
#include <sstream>

namespace pcp {

int64_t getCurrentTimeMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

static std::string formatTime(time_t t) {
    char buf[16];
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, sizeof(buf), "%H:%M", &tm);
    return buf;
}

std::string formatEventTime(int64_t ms) { return formatRelativeTimestamp(ms); }

std::string formatTimelineTimestamp(int64_t ms) {
    time_t t = ms / 1000;
    if (isToday(ms)) return formatTime(t);
    if (isYesterday(ms)) return "Yesterday " + formatTime(t);
    struct tm tm;
    localtime_r(&t, &tm);
    char buf[32];
    strftime(buf, sizeof(buf), "%b %d, %H:%M", &tm);
    return buf;
}

std::string formatDetailTimestamp(int64_t ms) {
    time_t t = ms / 1000;
    char buf[64];
    struct tm tm;
    localtime_r(&t, &tm);
    strftime(buf, sizeof(buf), "%B %d, %Y %H:%M:%S", &tm);
    return buf;
}

std::string formatRelativeTimestamp(int64_t ms) {
    int64_t now = getCurrentTimeMs();
    int64_t diff = now - ms;
    if (diff < 60000) return "just now";
    if (diff < 3600000) return std::to_string(diff / 60000) + "m ago";
    if (diff < 86400000) return std::to_string(diff / 3600000) + "h ago";
    if (diff < 172800000) return "Yesterday";
    if (diff < 604800000) return std::to_string(diff / 86400000) + "d ago";
    return formatTimelineTimestamp(ms);
}

bool isSameDay(int64_t ts1, int64_t ts2) {
    time_t t1 = ts1 / 1000, t2 = ts2 / 1000;
    struct tm tm1, tm2;
    localtime_r(&t1, &tm1); localtime_r(&t2, &tm2);
    return tm1.tm_yday == tm2.tm_yday && tm1.tm_year == tm2.tm_year;
}

bool isToday(int64_t ms) { return isSameDay(ms, getCurrentTimeMs()); }

bool isYesterday(int64_t ms) {
    return isSameDay(ms, getCurrentTimeMs() - 86400000);
}

bool isRecent(int64_t ms, int minutes) {
    return (getCurrentTimeMs() - ms) < (minutes * 60000LL);
}

} // namespace pcp
