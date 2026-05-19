#include "progressive/date_utils.hpp"
#include <sstream>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <cstring>

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

// Original Kotlin: formatTimestamp — dispatch to correct formatter based on enum
std::string formatTimestamp(int64_t epochMs, DateFormat fmt, int64_t nowMsRef) {
    if (epochMs <= 0) return "";
    switch (fmt) {
        case DateFormat::ISO_8601:
            return formatIso8601(epochMs);
        case DateFormat::RFC_2822: {
            time_t ts = epochMs / 1000;
            struct tm result;
            gmtime_r(&ts, &result);
            char buf[64];
            strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S +0000", &result);
            return std::string(buf);
        }
        case DateFormat::SHORT_DATE: {
            time_t ts = epochMs / 1000;
            struct tm result;
            gmtime_r(&ts, &result);
            char buf[16];
            strftime(buf, sizeof(buf), "%m/%d/%y", &result);
            return std::string(buf);
        }
        case DateFormat::LONG_DATE:
            return formatDate(epochMs);
        case DateFormat::SHORT_TIME:
            return formatTime(epochMs, false);
        case DateFormat::LONG_TIME:
            return formatTime(epochMs, true);
        case DateFormat::SHORT_DATETIME: {
            time_t ts = epochMs / 1000;
            struct tm result;
            gmtime_r(&ts, &result);
            char buf[32];
            strftime(buf, sizeof(buf), "%m/%d/%y %H:%M", &result);
            return std::string(buf);
        }
        case DateFormat::LONG_DATETIME:
            return formatChatTimestamp(epochMs);
        case DateFormat::RELATIVE:
            return formatRelativeTime(epochMs, nowMsRef);
        case DateFormat::RELATIVE_SHORT:
            return formatRelativeShort(epochMs, nowMsRef);
        case DateFormat::DURATION:
            return formatDuration(epochMs);
    }
    return "";
}

// Original Kotlin: formatRelativeShort — compact relative: "5m", "2h", "1d"
std::string formatRelativeShort(int64_t epochMs, int64_t nowMsRef) {
    if (epochMs <= 0) return "";
    if (nowMsRef <= 0) nowMsRef = nowMs();
    int64_t diffMs = nowMsRef - epochMs;
    if (diffMs < 0) return "0s";
    int64_t seconds = diffMs / 1000;
    int64_t minutes = seconds / 60;
    int64_t hours = minutes / 60;
    int64_t days = hours / 24;
    int64_t weeks = days / 7;
    int64_t months = days / 30;
    int64_t years = days / 365;

    if (seconds < 60) return "just now";
    if (minutes < 60) return std::to_string(minutes) + "m ago";
    if (hours < 24) return std::to_string(hours) + "h ago";
    if (days == 1) return "yesterday";
    if (days < 7) return std::to_string(days) + "d ago";
    if (weeks < 4) return std::to_string(weeks) + "w ago";
    if (months < 12) return std::to_string(months) + "mo ago";
    return std::to_string(years) + "y ago";
}

// Original Kotlin: parseRfc2822 — parse RFC 2822 date to epoch ms
int64_t parseRfc2822(const std::string& rfcDate) {
    // Format: "Mon, 13 May 2025 12:34:56 +0000" or similar
    struct tm t = {};
    // Skip day-of-week prefix if present
    const char* p = rfcDate.c_str();
    if (rfcDate.find(',') != std::string::npos) {
        p = rfcDate.c_str() + rfcDate.find(',') + 2; // skip "Mon, "
    }
    int year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
    char monStr[4] = {};
    if (sscanf(p, "%d %3s %d %d:%d:%d", &day, monStr, &year, &hour, &min, &sec) >= 3) {
        static const char* months[] = {
            "Jan","Feb","Mar","Apr","May","Jun",
            "Jul","Aug","Sep","Oct","Nov","Dec"
        };
        for (int i = 0; i < 12; i++) {
            if (strncmp(monStr, months[i], 3) == 0) { month = i + 1; break; }
        }
        if (month == 0) return 0;
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

// Original Kotlin: isDateInRange — check if timestamp falls in range
bool isDateInRange(int64_t epochMs, const DateRange& range) {
    return epochMs >= range.start && epochMs <= range.end;
}

// Original Kotlin: daysBetween — calendar days between two timestamps
int daysBetween(int64_t startMs, int64_t endMs) {
    if (startMs > endMs) return daysBetween(endMs, startMs);
    int64_t dayMs = 86400000LL;
    int64_t startDay = startMs / dayMs;
    int64_t endDay = endMs / dayMs;
    return static_cast<int>(endDay - startDay);
}

// Original Kotlin: getStartOfDay — midnight UTC of given timestamp
int64_t getStartOfDay(int64_t epochMs) {
    int64_t dayMs = 86400000LL;
    return (epochMs / dayMs) * dayMs;
}

// Original Kotlin: getEndOfDay — 23:59:59.999 UTC
int64_t getEndOfDay(int64_t epochMs) {
    return getStartOfDay(epochMs) + 86399999LL;
}

// Original Kotlin: getStartOfWeek — previous Monday 00:00 UTC
int64_t getStartOfWeek(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    int daysFromMonday = (result.tm_wday + 6) % 7; // Sunday=0 -> Monday=0
    return getStartOfDay(epochMs) - daysFromMonday * 86400000LL;
}

// Original Kotlin: getStartOfMonth — 1st of month 00:00 UTC
int64_t getStartOfMonth(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    result.tm_mday = 1;
    result.tm_hour = 0;
    result.tm_min = 0;
    result.tm_sec = 0;
    return static_cast<int64_t>(timegm(&result)) * 1000;
}

// Original Kotlin: isThisMonth — check if timestamp is in current month
bool isThisMonth(int64_t epochMs) {
    time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    time_t ts = epochMs / 1000;
    struct tm nowTm, tsTm;
    gmtime_r(&now, &nowTm);
    gmtime_r(&ts, &tsTm);
    return nowTm.tm_year == tsTm.tm_year && nowTm.tm_mon == tsTm.tm_mon;
}

// Original Kotlin: buildCalendarMonth — create CalendarMonth struct
CalendarMonth buildCalendarMonth(int year, int month) {
    CalendarMonth cal;
    cal.year = year;
    cal.month = month;
    cal.name = getMonthName(month);

    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    cal.days = daysInMonth[month - 1];
    if (month == 2 && ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) {
        cal.days = 29;
    }

    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = 1;
    t.tm_hour = 12;
    t.tm_min = 0;
    t.tm_sec = 0;
    if (timegm(&t) != -1) {
        cal.firstDayOffset = t.tm_wday;
    }

    return cal;
}

// Original Kotlin: abbreviated weekday names
static const char* SHORT_DAYS[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};

// Original Kotlin: abbreviated month names
static const char* SHORT_MONTHS[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

// Original Kotlin: getShortDayName — abbreviated day: "Mon"
std::string getShortDayName(int64_t epochMs) {
    if (epochMs <= 0) return "";
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return SHORT_DAYS[result.tm_wday];
}

// Original Kotlin: getShortMonthName — abbreviated month: "Jan"
std::string getShortMonthName(int month) {
    if (month < 1 || month > 12) return "";
    return SHORT_MONTHS[month - 1];
}

// Original Kotlin: addDays — offset timestamp by N days
int64_t addDays(int64_t epochMs, int days) {
    return epochMs + static_cast<int64_t>(days) * 86400000LL;
}

// Original Kotlin: addMonths — offset timestamp by N months
int64_t addMonths(int64_t epochMs, int months) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    int totalMonths = result.tm_year * 12 + (result.tm_mon) + months;
    result.tm_year = totalMonths / 12;
    result.tm_mon = totalMonths % 12;
    return static_cast<int64_t>(timegm(&result)) * 1000;
}

// Original Kotlin: addYears — offset timestamp by N years
int64_t addYears(int64_t epochMs, int years) {
    return addMonths(epochMs, years * 12);
}

// Original Kotlin: getYear — extract year component
int getYear(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_year + 1900;
}

// Original Kotlin: getMonth — extract month component (1-12)
int getMonth(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_mon + 1;
}

// Original Kotlin: getDayOfMonth — extract day component (1-31)
int getDayOfMonth(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_mday;
}

// Original Kotlin: getDayOfWeek — extract weekday (0=Sunday, 6=Saturday)
int getDayOfWeek(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_wday;
}

// Original Kotlin: getHour — extract hour component (0-23)
int getHour(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_hour;
}

// Original Kotlin: getMinute — extract minute component (0-59)
int getMinute(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_min;
}

// Original Kotlin: isSameDay — check if two timestamps fall on same calendar day
bool isSameDay(int64_t epochMsA, int64_t epochMsB) {
    time_t ta = epochMsA / 1000, tb = epochMsB / 1000;
    struct tm tma, tmb;
    gmtime_r(&ta, &tma);
    gmtime_r(&tb, &tmb);
    return tma.tm_year == tmb.tm_year && tma.tm_mon == tmb.tm_mon && tma.tm_mday == tmb.tm_mday;
}

// Original Kotlin: formatDurationHMS — duration as H:MM:SS
std::string formatDurationHMS(int64_t durationMs) {
    if (durationMs <= 0) return "0:00";
    int64_t totalSec = durationMs / 1000;
    int hours = totalSec / 3600;
    int minutes = (totalSec % 3600) / 60;
    int seconds = totalSec % 60;
    char buf[16];
    if (hours > 0) {
        snprintf(buf, sizeof(buf), "%d:%02d:%02d", hours, minutes, seconds);
    } else {
        snprintf(buf, sizeof(buf), "%d:%02d", minutes, seconds);
    }
    return std::string(buf);
}

// Original Kotlin: getWeekOfYear — ISO week number (1-53)
int getWeekOfYear(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[8];
    strftime(buf, sizeof(buf), "%V", &result);
    return std::atoi(buf);
}

// Original Kotlin: getQuarter — quarter of year (1-4)
int getQuarter(int month) {
    if (month < 1 || month > 12) return 0;
    return (month - 1) / 3 + 1;
}

// Original Kotlin: getAge — age in years from birth timestamp
int getAge(int64_t birthEpochMs, int64_t nowMsRef) {
    if (birthEpochMs <= 0) return 0;
    if (nowMsRef <= 0) nowMsRef = nowMs();
    int birthYear = getYear(birthEpochMs);
    int birthMonth = getMonth(birthEpochMs);
    int birthDay = getDayOfMonth(birthEpochMs);
    int nowYear = getYear(nowMsRef);
    int nowMonth = getMonth(nowMsRef);
    int nowDay = getDayOfMonth(nowMsRef);
    int age = nowYear - birthYear;
    if (nowMonth < birthMonth || (nowMonth == birthMonth && nowDay < birthDay)) {
        age--;
    }
    return age < 0 ? 0 : age;
}

// Original Kotlin: getDayOfYear — day of year (1-366)
int getDayOfYear(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    return result.tm_yday + 1;
}

// Original Kotlin: isLeapYear — check if year is leap year
bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

// Original Kotlin: daysInMonth — days in given month/year
int daysInMonth(int year, int month) {
    static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month < 1 || month > 12) return 0;
    if (month == 2 && isLeapYear(year)) return 29;
    return d[month - 1];
}

// Original Kotlin: getStartOfYear — Jan 1 00:00 UTC for given year
int64_t getStartOfYear(int year) {
    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = 0;
    t.tm_mday = 1;
    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;
    return static_cast<int64_t>(timegm(&t)) * 1000;
}

// Original Kotlin: getEndOfMonth — last millisecond of month
int64_t getEndOfMonth(int64_t epochMs) {
    int year = getYear(epochMs);
    int month = getMonth(epochMs);
    struct tm t = {};
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = daysInMonth(year, month);
    t.tm_hour = 23;
    t.tm_min = 59;
    t.tm_sec = 59;
    return static_cast<int64_t>(timegm(&t)) * 1000 + 999;
}

// Original Kotlin: diffDays — signed days between two timestamps
int diffDays(int64_t fromMs, int64_t toMs) {
    int64_t dayMs = 86400000LL;
    return static_cast<int>((toMs / dayMs) - (fromMs / dayMs));
}

// Original Kotlin: diffHours — signed hours between two timestamps
int diffHours(int64_t fromMs, int64_t toMs) {
    int64_t hourMs = 3600000LL;
    return static_cast<int>((toMs / hourMs) - (fromMs / hourMs));
}

// Original Kotlin: diffMinutes — signed minutes between two timestamps
int diffMinutes(int64_t fromMs, int64_t toMs) {
    int64_t minMs = 60000LL;
    return static_cast<int>((toMs / minMs) - (fromMs / minMs));
}

// Original Kotlin: roundToNearestMinute — round epoch ms to nearest minute
int64_t roundToNearestMinute(int64_t epochMs) {
    int64_t minMs = 60000LL;
    return ((epochMs + minMs / 2) / minMs) * minMs;
}

// Original Kotlin: roundToNearestHour — round epoch ms to nearest hour
int64_t roundToNearestHour(int64_t epochMs) {
    int64_t hourMs = 3600000LL;
    return ((epochMs + hourMs / 2) / hourMs) * hourMs;
}

// Original Kotlin: formatMonthYear — "May 2025"
std::string formatMonthYear(int64_t epochMs) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    char buf[32];
    strftime(buf, sizeof(buf), "%B %Y", &result);
    return std::string(buf);
}

// Original Kotlin: getMillisUntil — ms from nowMsRef to targetMs (negative if past)
int64_t getMillisUntil(int64_t targetMs, int64_t nowMsRef) {
    if (nowMsRef <= 0) nowMsRef = nowMs();
    return targetMs - nowMsRef;
}

// Original Kotlin: getTimeComponents — extract h/m/s into references
void getTimeComponents(int64_t epochMs, int& hour, int& minute, int& second) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    hour = result.tm_hour;
    minute = result.tm_min;
    second = result.tm_sec;
}

// Original Kotlin: getDateComponents — extract y/m/d into references
void getDateComponents(int64_t epochMs, int& year, int& month, int& day) {
    time_t ts = epochMs / 1000;
    struct tm result;
    gmtime_r(&ts, &result);
    year = result.tm_year + 1900;
    month = result.tm_mon + 1;
    day = result.tm_mday;
}

// Original Kotlin: formatRelativeTimeVerbose — more descriptive relative time
std::string formatRelativeTimeVerbose(int64_t epochMs, int64_t nowMsRef) {
    if (epochMs <= 0) return "";
    if (nowMsRef <= 0) nowMsRef = nowMs();
    int64_t diffMs = nowMsRef - epochMs;
    if (diffMs < 0) {
        int64_t aheadMs = -diffMs;
        int64_t sec = aheadMs / 1000;
        if (sec < 60) return "in a few seconds";
        if (sec < 3600) return "in " + std::to_string(sec / 60) + " minute(s)";
        if (sec < 86400) return "in " + std::to_string(sec / 3600) + " hour(s)";
        return "in " + std::to_string(sec / 86400) + " day(s)";
    }
    int64_t sec = diffMs / 1000;
    if (sec < 10) return "just now";
    if (sec < 60) return std::to_string(sec) + " seconds ago";
    if (sec < 120) return "a minute ago";
    if (sec < 3600) return std::to_string(sec / 60) + " minutes ago";
    if (sec < 7200) return "an hour ago";
    if (sec < 86400) return std::to_string(sec / 3600) + " hours ago";
    if (sec < 172800) return "yesterday";
    int days = sec / 86400;
    if (days < 7) return std::to_string(days) + " days ago";
    if (days < 14) return "a week ago";
    if (days < 60) return std::to_string(days / 7) + " weeks ago";
    if (days < 365) return std::to_string(days / 30) + " months ago";
    if (days < 730) return "a year ago";
    return std::to_string(days / 365) + " years ago";
}

} // namespace progressive
