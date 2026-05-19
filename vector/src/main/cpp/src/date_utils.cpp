#include "progressive/date_utils.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <chrono>

namespace progressive {

static const char* MONTHS[] = {
    "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};

static const char* DAYS[] = {
    "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};

std::string formatChatTimestamp(int64_t epochMs, bool includeSeconds) {
    if (epochMs <= 0) return "";

    if (isToday(epochMs)) {
        return formatTime(epochMs, includeSeconds);
    }

    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);

    char buf[64];

    if (isYesterday(epochMs)) {
        char timeBuf[16];
        strftime(timeBuf, sizeof(timeBuf), includeSeconds ? "%H:%M:%S" : "%H:%M", &result);
        std::ostringstream out;
        out << "Yesterday " << timeBuf;
        return out.str();
    }

    if (isThisWeek(epochMs)) {
        char timeBuf[16];
        strftime(timeBuf, sizeof(timeBuf), includeSeconds ? "%H:%M:%S" : "%H:%M", &result);
        std::ostringstream out;
        out << DAYS[result.tm_wday] << " " << timeBuf;
        return out.str();
    }

    if (isThisYear(epochMs)) {
        strftime(buf, sizeof(buf), includeSeconds ? "%b %d %H:%M:%S" : "%b %d %H:%M", &result);
    } else {
        strftime(buf, sizeof(buf), includeSeconds ? "%b %d, %Y %H:%M:%S" : "%b %d, %Y %H:%M", &result);
    }

    return std::string(buf);
}

std::string formatDate(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[32];
    strftime(buf, sizeof(buf), "%B %d, %Y", &result);
    return std::string(buf);
}

std::string formatTime(int64_t epochMs, bool includeSeconds) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[16];
    strftime(buf, sizeof(buf), includeSeconds ? "%H:%M:%S" : "%H:%M", &result);
    return std::string(buf);
}

std::string formatIso8601(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%dT%H:%M:%SZ", &result);
    return std::string(buf);
}

std::string formatRelativeTime(int64_t epochMs, int64_t nowMsVal) {
    if (epochMs <= 0) return "";
    if (nowMsVal <= 0) nowMsVal = nowMs();

    int64_t diffMs = nowMsVal - epochMs;
    if (diffMs < 0) return "in the future";

    int64_t seconds = diffMs / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;

    if (seconds < 60) return "just now";
    if (minutes == 1) return "1 minute ago";
    if (minutes < 60) return std::to_string(minutes) + " minutes ago";
    if (hours == 1) return "1 hour ago";
    if (hours < 24) return std::to_string(hours) + " hours ago";
    if (days == 1) return "1 day ago";
    if (days < 30) return std::to_string(days) + " days ago";
    if (days < 365) return std::to_string(days / 30) + " months ago";
    return std::to_string(days / 365) + " years ago";
}

std::string formatDuration(int64_t durationMs) {
    if (durationMs <= 0) return "0s";
    int64_t totalSec = durationMs / 1000;
    int days = totalSec / 86400;
    int hours = (totalSec % 86400) / 3600;
    int minutes = (totalSec % 3600) / 60;

    std::ostringstream out;
    if (days > 0) out << days << "d ";
    if (hours > 0) out << hours << "h ";
    out << minutes << "m";
    return out.str();
}

int64_t parseIso8601(const std::string& isoDate) {
    // Format: 2025-05-13T12:34:56Z or 2025-05-13T12:34:56+00:00
    struct tm t = {};
    int year, month, day, hour = 0, min = 0, sec = 0;

    if (sscanf(isoDate.c_str(), "%d-%d-%dT%d:%d:%d", &year, &month, &day, &hour, &min, &sec) >= 3) {
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_min = min;
        t.tm_sec = sec;
        return static_cast<int64_t>(timegm(&t)) * 1000;
    }
    return 0;
}

int64_t nowMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

bool isToday(int64_t epochMs) {
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    time_t ts = epochMs / 1000;
    struct tm nowTm, tsTm;
    gmtime_r(&now, &nowTm);
    gmtime_r(&ts, &tsTm);
    return nowTm.tm_year == tsTm.tm_year &&
           nowTm.tm_mon == tsTm.tm_mon &&
           nowTm.tm_mday == tsTm.tm_mday;
}

bool isYesterday(int64_t epochMs) {
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    time_t yesterday = now - 86400;
    time_t ts = epochMs / 1000;
    struct tm yesterdayTm, tsTm;
    gmtime_r(&yesterday, &yesterdayTm);
    gmtime_r(&ts, &tsTm);
    return yesterdayTm.tm_year == tsTm.tm_year &&
           yesterdayTm.tm_mon == tsTm.tm_mon &&
           yesterdayTm.tm_mday == tsTm.tm_mday;
}

bool isThisWeek(int64_t epochMs) {
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    time_t ts = epochMs / 1000;
    double diffSec = difftime(now, ts);
    return diffSec >= 0 && diffSec < 7 * 86400;
}

bool isThisYear(int64_t epochMs) {
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    time_t ts = epochMs / 1000;
    struct tm nowTm, tsTm;
    gmtime_r(&now, &nowTm);
    gmtime_r(&ts, &tsTm);
    return nowTm.tm_year == tsTm.tm_year;
}

std::string getDayName(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return DAYS[result.tm_wday];
}

std::string getMonthName(int month) {
    if (month < 1 || month > 12) return "";
    return MONTHS[month - 1];
}

} // namespace progressive
